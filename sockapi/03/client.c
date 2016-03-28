// 

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

int main(void) {
	
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		ERR_EXIT("socket error.\n");
	}
	
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");	
	if (connect(sockfd, (const struct sockaddr *)&srvaddr, sizeof(srvaddr)) == -1) {
		ERR_EXIT("connect error.\n");
	}
	
	char recvbuf[1024] = {0};
	char sendbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
		int datalen;
		if (write(sockfd, sendbuf, strlen(sendbuf)) == -1) {
			close(sockfd);
			ERR_EXIT("write error.\n");
		}
		
		datalen = read(sockfd, recvbuf, sizeof(recvbuf));
		if (datalen == 0) { // 服务器端发送了FIN
			printf("server closed.\n");
			close(sockfd);
			exit(0);
		}
		else if (datalen < 0) {
			close(sockfd);
			ERR_EXIT("read error.\n");
		}
		fputs(recvbuf, stdout);
		
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}
	
	return 0;
}