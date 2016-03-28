// 2、实现支持多客户端连接的服务器

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include <sys/wait.h>

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
			
void myhandler(int sig) {
	printf("recv a signal: %d\n", sig);
	int mypid = 0;
	while ( (mypid = waitpid(-1, NULL, WNOHANG)) > 0) {
		;
	}
}

int main(void) {
	
	signal(SIGCHLD, myhandler);
	
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
	
	while (1) {
		struct sockaddr_in peeraddr; // 
		socklen_t peerlen;
		int conn;
		if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) == -1) {
			ERR_EXIT("accept error.\n");
		}
		printf("peer address: %s\n", inet_ntoa(peeraddr.sin_addr));
		printf("peer port: %d\n", ntohs(peeraddr.sin_port));
		
		pid_t pid;
		pid = fork();
		if (pid == -1) {
			printf("fork failed.\n");
			continue;
		}
		else if (pid == 0) { // 子进程处理连接
			close(listenfd);
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
		}
		else if (pid > 0) { // 父进程
			close(conn);
		}
	}
	
	close(listenfd);
	
	return 0;
}