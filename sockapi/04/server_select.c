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
	if (sig == SIGCHLD)
	{
		int pid = 0;
		while ( (pid = waitpid(0, NULL, WNOHANG)) > 0)
		{
			printf("child proc exit: %d\n", pid);
			fflush(stdout);
		}
	}
	else if (sig == SIGPIPE)
	{
		;
	}
}

int main(void)
{
	int 			ret = 0;
	int 			listenfd = -1;
	
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket error.\n");
		exit(0);
	}
	
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(listenfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1) {
		perror("bind error.\n");
		exit(0);
	}
	
	// 设置地址复用
	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                      &on, sizeof(on)) < 0) {
		perror("setsockopt error.\n");
		exit(0);
	}
	
	if (listen(listenfd, SOMAXCONN) == -1) {
		perror("listen error.\n");
		exit(0);
	}
	
	struct sockaddr_in peeraddr;
	socklen_t peerlen;
	int conn;
	
	int i;
	int client[FD_SETSIZE];
	int maxi = 0;
	
	printf("FD_SETSIZE: %d\n", FD_SETSIZE);
	for (i = 0; i < FD_SETSIZE; ++i)
		client[i] = -1;
	
	int nready;
	int maxfd = listenfd;
	fd_set		rset;
	fd_set		allset;
	
	FD_ZERO(&rset);
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	printf("FD_SETSIZE: %d\n", FD_SETSIZE);
	
	while (1)
	{
		rset = allset;
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if (nready == -1)
		{
			if (errno == EINTR)
				continue;
			perror("select error\n");
			exit(0);
		}
		if (nready == 0)
			continue;
		
		if (FD_ISSET(listenfd, &rset))
		{
			peerlen = sizeof(peeraddr);
			conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen);
			if (conn == -1)
			{
				perror("accept error");
				exit(0);
			}
			for (i = 0; i < FD_SETSIZE; ++i)
			{
				if (client[i] < 0)
				{
					client[i] = conn;
					if (i > maxi)
						maxi = i;
					break;
				}
			}
			if (i == FD_SETSIZE)
			{
				fprintf(stderr, "too many clients.\n");
				exit(EXIT_FAILURE);
			}
			printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
			
			FD_SET(conn, &allset);
			if (conn > maxfd)
				maxfd = conn;
			if (--nready <= 0)
				continue;
			
		}
		
		for (i = 0; i <= maxi; ++i)
		{
			conn = client[i];
			if (conn == -1)
				continue;
			if (FD_ISSET(conn, &rset))
			{
				char recvbuf[1024] = {0};
				int ret = readline(conn, recvbuf, sizeof(recvbuf));
				if (ret == -1)
				{
					perror("readline error.\n");
					exit(0);
				}
				if (ret == 0)
				{
					printf("client closed\n");
					FD_CLR(conn, &allset);
					client[i] = -1;
					close(conn);
				}
				fputs(recvbuf, stdout);
				writen(conn, recvbuf, strlen(recvbuf));
				
				if (--nready <= 0)
					break;
			}
		}
	}
	
	return ret;
}