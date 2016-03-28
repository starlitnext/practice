#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>


/*
int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

       void FD_CLR(int fd, fd_set *set);
       int  FD_ISSET(int fd, fd_set *set);
       void FD_SET(int fd, fd_set *set);
       void FD_ZERO(fd_set *set);

*/

ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nread;
	char *bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (nread == 0)
			return count - nleft;

		bufp += nread;
		nleft -= nread;
	}

	return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char *bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (nwritten == 0)
			continue;

		bufp += nwritten;
		nleft -= nwritten;
	}

	return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while (1)
	{
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread;
	char *bufp = buf;
	int nleft = maxline;
	while (1)
	{
		ret = recv_peek(sockfd, bufp, nleft);
		if (ret < 0)
			return ret;
		else if (ret == 0)
			return ret;

		nread = ret;
		int i;
		for (i=0; i<nread; i++)
		{
			if (bufp[i] == '\n')
			{
				ret = readn(sockfd, bufp, i+1);
				if (ret != i+1)
					exit(EXIT_FAILURE);

				return ret;
			}
		}

		if (nread > nleft)
			exit(EXIT_FAILURE);

		nleft -= nread;
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
			exit(EXIT_FAILURE);

		bufp += nread;
	}

	return -1;
}

void myhandle(int sig)
{
	printf("rec a sig: %d\n", sig);
	if (sig == SIGPIPE)
	{
		;
	}
}

int main(void)
{
	int 			ret = 0;
	int 			sockfd = -1;
	
	signal(SIGPIPE, myhandle);
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		printf("socket err.\n");
		exit(0);
	}
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8001);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");	
	if (connect(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("connect err.\n");
		exit(0);
	}
	
	int stdinfd = fileno(stdin);
	int maxfd = stdinfd < sockfd ? sockfd : stdinfd;
	fd_set fdset, rset;
	FD_ZERO(&fdset);
	FD_SET(stdinfd, &fdset);
	FD_SET(sockfd, &fdset);
	
	char 	recvbuf[1024] = {0};
	int 	recvlen = 1024;
	char 	sendbuf[1024] = {0};
	int 	sendlen = 1024;
	while (1)
	{
		rset = fdset;
		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		int ret = 0;
		ret = select(maxfd + 1, &rset, NULL, NULL, &timeout);
		if (ret == -1)
		{
			if (errno == EINTR || errno == EINPROGRESS)
				continue;
			else 
			{
				printf("connect err: %d\n", errno);
				exit(0);
			}
		}
		else if (ret == 0)
		{
			printf("timeout...\n");
			continue;
		}
		
		if (FD_ISSET(stdinfd, &rset))
		{
			fgets(sendbuf, sizeof(sendbuf), stdin);
			sendlen = strlen(sendbuf);
			ret = writen(sockfd, sendbuf, sendlen);
			if (ret != sendlen)
			{
				printf("write error: %d\n", ret);
				break;
			}
			memset(sendbuf, 0, sizeof(sendbuf));
		}
		if (FD_ISSET(sockfd, &rset))
		{
			recvlen = readline(sockfd, recvbuf, sizeof(recvbuf));
			if (recvlen == 0) 
			{
				printf("peer closed: %d\n", ret);
				break;
			}
			else if (recvlen == -1)
			{
				printf("read error: %d\n", ret);
				break;
			}
			fputs(recvbuf, stdout);
			memset(recvbuf, 0, sizeof(recvbuf));
		}
	}
	
	close(sockfd);	
	
	return ret;
}