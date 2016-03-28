#ifndef _COMMON_SOCKET_H
#define _COMMON_SOCKET_H

#ifdef __cplusplus
extern 'C'
{
#endif

// �����붨��
#define Sck_Ok				0
#define Sck_BaseErr			3000

#define Sck_ErrParam				(Sck_BaseErr+1)
#define Sck_ErrTimeOut				(Sck_BaseErr+2)
#define Sck_ErrPeerClosed			(Sck_BaseErr+3)
#define Sck_ErrMalloc				(Sck_BaseErr+4)

// --------------------�ͻ���API-------------------
// �ͻ��˻�����ʼ��
// int sckClient_init(void **handle, int contime, int sendtime, int revtime, int connCnt);
int sckClient_init(void **handle, int contime, int sendtime, int revtime, int nConNum);

int sckClient_getconn(void *handle, char *ip, int port, int *connfd);

int sckClient_putconn(void *handle, int *connfd);

// �ͻ��˷��ͱ���
int sckClient_send(void *handle, int connfd, char *data, int datalen);

// �ͻ��˽��ձ���
int sckClient_rev(void *handle, int connfd, char *out, int *outlen);

// �ͻ��˻����ͷ�
int sckClient_destory(void *handle);

// --------------------�ͻ���API-------------------

// --------------------��������API-------------------

// �������˻�����ʼ��
int sckServer_init(int port, int *listenfd);

// �������˻�ȡ����
int sckServer_accept(int listenfd, int *connfd, int timeout);

// �������˷��ͱ���
int sckServer_send(int connfd, char *data, int datalen, int timeout);

// �������˽��ձ���
int sckServer_rev(int connfd, char *out, int *outlen, int timeout);

// �������˻����ͷ�
int sckServer_destory(int listenfd);


// --------------------��������API-------------------

#ifdef __cplusplus
}
#endif

#endif // _COMMON_SOCKET_H