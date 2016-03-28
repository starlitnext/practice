#include "commsocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int 			ret = 0;
	int 			port = 8001;
	int 			listenfd = -1;
	int				connfd = -1;
	// char			data[1024] = {0};
	// int 			datalen = 1024;
	char			out[1024] = {0};
	int 			outlen = 1024;
	// 服务器端环境初始化
	ret = sckServer_init(port, &listenfd);
	if (ret != 0) {
		printf("func sckServer_init() err: %d\n", ret);
		return ret;
	}
	
	// 服务器端获取连接
	ret = sckServer_accept(listenfd, &connfd, 5);
	if (ret != 0) {
		printf("func sckServer_accept() err: %d\n", ret);
		return ret;
	}

	while (1) {
		
		// 服务器端接收报文
		ret = sckServer_rev(connfd, out, &outlen, 5);
		if (ret != 0) {
			printf("func sckServer_rev() err: %d\n", ret);
			return ret;
		}
		
		fputs(out, stdout);

		// 服务器端发送报文
		ret = sckServer_send(connfd, out, sizeof(out), 5);
		if (ret != 0) {
			printf("func sckServer_send() err: %d\n", ret);
			return ret;
		}
		
		memset(out, 0, sizeof(out));
	}
	
	// 服务器端环境释放
	ret = sckServer_destory(listenfd);
	
	return ret;
}