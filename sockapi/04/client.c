#include "commsocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) 
{
	int 			ret = 0;
	void 			*handle = NULL;
	int				connfd = -1;
	
	char			data[1024] = {0};
	int 			datalen = 1024;
	char			out[1024] = {0};
	int 			outlen = 1024;
	// 客户端环境初始化
	ret = sckClient_init(&handle, 5, 5, 5, 1);
	if (ret != 0) {
		printf("func sckClient_init() err: %d\n", ret);
		return ret;
	}
	
	ret = sckClient_getconn(handle, "127.0.0.1", 8001, &connfd);
	if (ret != 0) {
		printf("func sckClient_init() err: %d\n", ret);
		return ret;
	}

	
	while (fgets(data, sizeof(data), stdin))
	{
		// 客户端发送报文
		datalen = strlen(data);
		// printf("datalen: %d, connfd: %d\n", datalen, connfd);
		ret = sckClient_send(handle, connfd, data, datalen);
		if (ret != 0) {
			if (ret == Sck_ErrTimeOut)
			{
				printf("send timeout, resend..");
				continue;
			}
			else {
				printf("func sckClient_send() err: %d\n", ret);
				goto DESTROY;
			}
				
		}
		
		// 客户端接收报文		
		ret = sckClient_rev(handle, connfd, out, &outlen);
		if (ret != 0) {
			printf("func sckClient_rev() err: %d\n", ret);
			goto DESTROY;
		}
		fputs(out, stdout);
		
		memset(data, 0, sizeof(data));
		memset(out, 0, sizeof(out));
	}	

DESTROY:
	// 客户端环境释放
	ret = sckClient_destory(handle);
	if (ret != 0) {
		printf("func sckClient_destory() err: %d\n", ret);
		return ret;
	}
	
	return 0;
}