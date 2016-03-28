#ifndef _COMMON_SOCKET_H
#define _COMMON_SOCKET_H

#ifdef __cplusplus
extern 'C'
{
#endif

// 错误码定义
#define Sck_Ok				0
#define Sck_BaseErr			3000

#define Sck_ErrParam				(Sck_BaseErr+1)
#define Sck_ErrTimeOut				(Sck_BaseErr+2)
#define Sck_ErrPeerClosed			(Sck_BaseErr+3)
#define Sck_ErrMalloc				(Sck_BaseErr+4)

// --------------------客户端API-------------------
// 客户端环境初始化
// int sckClient_init(void **handle, int contime, int sendtime, int revtime, int connCnt);
int sckClient_init(void **handle, int contime, int sendtime, int revtime, int nConNum);

int sckClient_getconn(void *handle, char *ip, int port, int *connfd);

int sckClient_putconn(void *handle, int *connfd);

// 客户端发送报文
int sckClient_send(void *handle, int connfd, char *data, int datalen);

// 客户端接收报文
int sckClient_rev(void *handle, int connfd, char *out, int *outlen);

// 客户端环境释放
int sckClient_destory(void *handle);

// --------------------客户端API-------------------

// --------------------服务器端API-------------------

// 服务器端环境初始化
int sckServer_init(int port, int *listenfd);

// 服务器端获取连接
int sckServer_accept(int listenfd, int *connfd, int timeout);

// 服务器端发送报文
int sckServer_send(int connfd, char *data, int datalen, int timeout);

// 服务器端接收报文
int sckServer_rev(int connfd, char *out, int *outlen, int timeout);

// 服务器端环境释放
int sckServer_destory(int listenfd);


// --------------------服务器端API-------------------

#ifdef __cplusplus
}
#endif

#endif // _COMMON_SOCKET_H