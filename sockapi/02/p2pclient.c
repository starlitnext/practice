// 

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
	pid_t mypid;
	while ( (mypid = waitpid(-1, NULL, WNOHANG)) > 0 ) {
		;
	}
	exit(0);
}

int main(void) {
	
	signal(SIGCHLD, myhandler);
	
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		ERR_EXIT("socket error.\n");
	}
	
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8002);
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");	
	if (connect(sockfd, (const struct sockaddr *)&srvaddr, sizeof(srvaddr)) == -1) {
		ERR_EXIT("connect error.\n");
	}
	
	// p2p �������ʵ��˼·��
	// accept��һ������֮��forkһ���ӽ��̣��ӽ�������������Ϣ������������������Ϣ
	
	pid_t pid;
	pid = fork();
	if (pid == -1) {
		ERR_EXIT("fork error.\n");
	}
	else if (pid == 0) { // child 
		char recvbuf[1024] = {0};
		while (1) {
			int datalen;
			datalen = read(sockfd, recvbuf, sizeof(recvbuf));
			if (datalen == 0) { // ��ʾ�ͻ��˷�����FIN 
				printf("client closed.\n");
				close(sockfd);
				exit(0);
			}
			else if (datalen < 0) {
				close(sockfd);
				ERR_EXIT("read error.\n");
			}
			
			fputs(recvbuf, stdout); // 
			memset(recvbuf, 0, sizeof(recvbuf));
		}
	}
	else if (pid > 0) {
		char sendbuf[1024] = {0};
		while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
			int datalen;
			datalen = write(sockfd, sendbuf, strlen(sendbuf)); // �������˷�������
			if (datalen < 0) {			
				close(sockfd);
				ERR_EXIT("write error.\n");
			}
			
			memset(sendbuf, 0, sizeof(sendbuf));
		}

	}
	
	close(sockfd);
	
	return 0;
}