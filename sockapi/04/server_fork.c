#include "commsocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


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
	int 			port = 8001;
	int 			listenfd = -1;
	
	signal(SIGCHLD, myhandle);
	signal(SIGPIPE, myhandle);
	
	// 服务器端环境初始化
	ret = sckServer_init(port, &listenfd);
	if (ret != 0) 
	{
		printf("func sckServer_init() err: %d\n", ret);
		return ret;
	}

	while (1) 
	{
		int				connfd = -1;
		int 			wait_seconds = 5;
		
		// 服务器端获取连接
		ret = sckServer_accept(listenfd, &connfd, wait_seconds);
		if (ret == Sck_ErrTimeOut) 
		{
			// printf("timeout...\n");
			continue;
		}
		printf("accept a connection.\n");
		
		int pid = fork();
		if (pid == 0) 
		{
			char 		recvbuf[1024];
			int			buflen = 1024;
			
			while (1) 
			{
				memset(recvbuf, 0, sizeof(recvbuf));
				// 服务器端接收报文
				ret = sckServer_rev(connfd, recvbuf, &buflen, wait_seconds);
				if (ret == Sck_ErrTimeOut) 
				{
					continue;
				}
				else if (ret != 0) 
				{
					printf("func sckServer_rev() err: %d\n", ret);
					break;
				}
				
				fputs(recvbuf, stdout);

				// 服务器端发送报文
				ret = sckServer_send(connfd, recvbuf, buflen, wait_seconds);
				if (ret != 0) 
				{
					printf("func sckServer_send() err: %d\n", ret);
					break;
				}
				
			}
			close(connfd);
			exit(ret);
		}
		else if (pid > 0) 
		{
			close(connfd);
		}
		
	}
	
	// 服务器端环境释放
	ret = sckServer_destory(listenfd);
	
	return ret;
}