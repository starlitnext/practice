// 处理粘包问题

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <error.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define ERR_EXIT(s) \
		do \
		{ \
			perror(s); \
			exit(EXIT_FAILURE); \
		} while (0)
			

/* read函数的调用方法
int ret;
ret = read_timeout(fd, 5);
if (ret == 0)
{
	read(fd, ...);
}
else if (ret == -1 && errno == ETIMEDOUT)
{
	timeout....
}
else
{
	ERR_EXIT("read_timeout");
}
*/

/**
 * read_timeout - 读超时检测函数，不含读操作
 * @fd: 文件描述符
 * @wait_seconds: 等待超时秒数，如果为0表示不检测超时
 * 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT
 */
int read_timeout(int fd, unsigned int wait_seconds)
{
	int ret = 0;
	if (wait_seconds > 0)
	{
		fd_set read_fdset;
		struct timeval timeout;

		FD_ZERO(&read_fdset);
		FD_SET(fd, &read_fdset);

		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		
		//select返回值三态
		//1 若timeout时间到（超时），没有检测到读事件 ret返回=0
		//2 若ret返回<0 &&  errno == EINTR 说明select的过程中被别的信号中断（可中断睡眠原理）
		//2-1 若返回-1，select出错
		//3 若ret返回值>0 表示有read事件发生，返回事件发生的个数
		
		do
		{
			ret = select(fd + 1, &read_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);

		if (ret == 0)
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
		else if (ret == 1)
			ret = 0;
	}

	return ret;
}
			
// 处理粘包问题
/**
 * readn - 读取固定字节数
 * @fd: 文件描述符
 * @buf: 接收缓冲区
 * @count: 要读取的字节数
 * 成功返回count，失败返回-1，读到EOF返回<count
*/
ssize_t readn(int fd, void *buf, size_t count) {
	ssize_t nleft = count;
	ssize_t nread;
	char *bufp = (char*)buf;
	
	while (nleft > 0) {
		if ((nread = read(fd, bufp, nleft)) < 0) {
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

/**
 * writen - 发送固定字节数
 * @fd: 文件描述符
 * @buf: 发送缓冲区
 * @count: 要读取的字节数
 * 成功返回count，失败返回-1
 */
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

/**
 * recv_peek - 仅仅查看套接字缓冲区数据，但不移除数据
 * @sockfd: 套接字
 * @buf: 接收缓冲区
 * @len: 长度
 * 成功返回>=0，失败返回-1
 */
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

/**
 * readline - 按行读取数据
 * @sockfd: 套接字
 * @buf: 接收缓冲区
 * @maxline: 每行最大长度
 * 成功返回>=0，失败返回-1
 */
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

int main(void) {
	
	int listenfd;
	
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		ERR_EXIT("socket error.\n");
	}
	
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(listenfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1) {
		ERR_EXIT("bind error.\n");
	}
	
	// 设置地址复用
	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                      &on, sizeof(on)) < 0) {
		ERR_EXIT("setsockopt error.\n");
	}
	
	if (listen(listenfd, SOMAXCONN) == -1) {
		ERR_EXIT("listen error.\n");
	}
	
	struct sockaddr_in peeraddr; // 
	socklen_t peerlen;
	int conn;
	if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) == -1) {
		ERR_EXIT("accept error.\n");
	}
	printf("peer address: %s\n", inet_ntoa(peeraddr.sin_addr));
	printf("peer port: %d\n", ntohs(peeraddr.sin_port));
	
	char recvbuf[1024] = {0};
	// char sendbuf[1024] = {0};
	while (1) {
		int datalen;
		datalen = read(conn, recvbuf, sizeof(recvbuf));
		if (datalen == 0) { // 表示客户端发送了FIN 
			printf("client closed.\n");
			close(conn);
			exit(0);
		}
		else if (datalen < 0) {
			close(conn);
			ERR_EXIT("read error.\n");
		}
		
		fputs(recvbuf, stdout); // 服务器端打印数据
		datalen = write(conn, recvbuf, datalen); // 服务器端发送数据
		if (datalen < 0) {			
			close(conn);
			ERR_EXIT("write error.\n");
		}
		
		memset(recvbuf, 0, sizeof(recvbuf));
	}

	close(listenfd);
	
	return 0;
}