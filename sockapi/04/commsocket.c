#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <error.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "commsocket.h"

typedef struct SckHandle
{
	int sockArray[100]; // ���ӳ�
	int arrayNum;
	int sockfd;
	int contime;
	int sendtime;
	int revtime;
}SckHandle;


// --------------------socket util-------------------

/**
 * readn - ��ȡ�̶��ֽ���
 * @fd: �ļ�������
 * @buf: ���ջ�����
 * @count: Ҫ��ȡ���ֽ���
 * �ɹ�����count��ʧ�ܷ���-1������EOF����<count
 */
static ssize_t readn(int fd, void *buf, size_t count)
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

/**
 * writen - ���͹̶��ֽ���
 * @fd: �ļ�������
 * @buf: ���ͻ�����
 * @count: Ҫ��ȡ���ֽ���
 * �ɹ�����count��ʧ�ܷ���-1
 */
static ssize_t writen(int fd, const void *buf, size_t count)
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
 * recv_peek - �����鿴�׽��ֻ��������ݣ������Ƴ�����
 * @sockfd: �׽���
 * @buf: ���ջ�����
 * @len: ����
 * �ɹ�����>=0��ʧ�ܷ���-1
 */
 /*
static ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while (1)
	{
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}
*/

/**
 * read_timeout - ����ʱ��⺯��������������
 * @fd: �ļ�������
 * @wait_seconds: �ȴ���ʱ���������Ϊ0��ʾ����ⳬʱ
 * �ɹ���δ��ʱ������0��ʧ�ܷ���-1����ʱ����-1����errno = ETIMEDOUT
 */
static int read_timeout(int fd, unsigned int wait_seconds)
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
		
		//select����ֵ��̬
		//1 ��timeoutʱ�䵽����ʱ����û�м�⵽���¼� ret����=0
		//2 ��ret����<0 &&  errno == EINTR ˵��select�Ĺ����б�����ź��жϣ����ж�˯��ԭ��
		//2-1 ������-1��select����
		//3 ��ret����ֵ>0 ��ʾ��read�¼������������¼������ĸ���
		
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

/**
 * write_timeout - д��ʱ��⺯��������д����
 * @fd: �ļ�������
 * @wait_seconds: �ȴ���ʱ���������Ϊ0��ʾ����ⳬʱ
 * �ɹ���δ��ʱ������0��ʧ�ܷ���-1����ʱ����-1����errno = ETIMEDOUT
 */
static int write_timeout(int fd, unsigned int wait_seconds)
{
	int ret = 0;
	if (wait_seconds > 0)
	{
		fd_set write_fdset;
		struct timeval timeout;

		FD_ZERO(&write_fdset);
		FD_SET(fd, &write_fdset);

		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			ret = select(fd + 1, NULL, &write_fdset, NULL, &timeout);
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

/**
 * activate_noblock - ����I/OΪ������ģʽ
 * @fd: �ļ������
 */
static int activate_nonblock(int fd)
{
	int ret = 0;
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1)
	{
		ret = flags;
		printf("func activate_nonblock() err: %d\n", ret);
		return ret;
	}

	flags |= O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, flags);
	if (ret == -1)
	{
		printf("func activate_nonblock() err: %d\n", ret);
		return ret;
	}
	return ret;
}

/**
 * deactivate_nonblock - ����I/OΪ����ģʽ
 * @fd: �ļ������
 */
static int deactivate_nonblock(int fd)
{
	int ret = 0;
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1)
	{
		ret = flags;
		printf("func deactivate_nonblock() err: %d\n", ret);
		return ret;
	}

	flags &= ~O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, flags);
	if (ret == -1)
	{
		printf("func deactivate_nonblock() err: %d\n", ret);
		return ret;
	}
	return ret;
}

/**
 * connect_timeout - connect
 * @fd: �׽���
 * @addr: Ҫ���ӵĶԷ���ַ
 * @wait_seconds: �ȴ���ʱ���������Ϊ0��ʾ����ģʽ
 * �ɹ���δ��ʱ������0��ʧ�ܷ���-1����ʱ����-1����errno = ETIMEDOUT
 */
static int connect_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret = 0;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_seconds > 0)
	{
		ret = activate_nonblock(fd);
		if (ret < 0)
			return ret;
	}
		
	ret = connect(fd, (struct sockaddr*)addr, addrlen);
	if (ret < 0 && errno == EINPROGRESS)
	{
		//printf("11111111111111111111\n");
		fd_set connect_fdset;
		struct timeval timeout;
		FD_ZERO(&connect_fdset);
		FD_SET(fd, &connect_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			// һ�����ӽ��������׽��־Ϳ�д  ����connect_fdset������д������
			ret = select(fd + 1, NULL, &connect_fdset, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == 0)
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
		else if (ret < 0)
			return -1;
		else if (ret == 1)
		{
			//printf("22222222222222222\n");
			/* ret����Ϊ1����ʾ�׽��ֿ�д�������������������һ�������ӽ����ɹ���һ�����׽��ֲ�������*/
			/* ��ʱ������Ϣ���ᱣ����errno�����У���ˣ���Ҫ����getsockopt����ȡ�� */
			int err;
			socklen_t socklen = sizeof(err);
			int sockoptret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &socklen);
			if (sockoptret == -1)
			{
				return -1;
			}
			if (err == 0)
			{
				//printf("3333333333333\n");
				ret = 0;
			}
			else
			{
				//printf("4444444444444444:%d\n", err);
				errno = err;
				ret = -1;
			}
		}
	}
	if (wait_seconds > 0)
	{
		ret = deactivate_nonblock(fd);
		if (ret < 0)
			return ret;
	}
	return ret;
}

/**
 * accept_timeout - ����ʱ��accept
 * @fd: �׽���
 * @addr: ������������ضԷ���ַ
 * @wait_seconds: �ȴ���ʱ���������Ϊ0��ʾ����ģʽ
 * �ɹ���δ��ʱ�������������׽��֣���ʱ����-1����errno = ETIMEDOUT
 */
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_seconds > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(fd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			ret = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == -1)
			return -1;
		else if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
	}

	//һ������ ��select�¼���������ʾ�Եȷ�������������֣��ͻ����������ӽ���
	//��ʱ�ٵ���accept���������
	if (addr != NULL)
		ret = accept(fd, (struct sockaddr*)addr, &addrlen); //�����������׽���
	else
		ret = accept(fd, NULL, NULL);
	if (ret == -1)
	{
		printf("func accept() err: %d\n", ret);
		return ret;
	}
	

	return ret;
}

// --------------------socket util-------------------

// --------------------�ͻ���API-------------------
// �ͻ��˻�����ʼ��
int sckClient_init(void **handle, int contime, int sendtime, int revtime, int nConNum)
{
	int ret = 0;
	if (handle == NULL || contime < 0 || sendtime < 0 || revtime < 0 || nConNum <= 0 || nConNum > 100) 
	{
		ret = Sck_ErrParam;
		printf("func sckClient_int() err %d: \
			check (handle == NULL || contime < 0 || sendtime < 0 || revtime < 0 || nConNum <= 0 || nConNum > 100)\n", ret);
		return ret;
	}
	
	SckHandle *tmp = (SckHandle*)malloc(sizeof(SckHandle));
	if (tmp == NULL) 
	{
		ret = Sck_ErrMalloc;
		printf("func sckClient_int() err %d: malloc\n", ret);
		return ret;
	}
	
	tmp->contime = contime;
	tmp->sendtime = sendtime;
	tmp->revtime = revtime;
	tmp->arrayNum = nConNum;
	
	*handle = tmp;
	
	return ret;
}

int sckClient_getconn(void *handle, char *ip, int port, int *connfd) 
{
	int ret = 0;
	struct SckHandle *tmp = NULL;
	if (handle == NULL || ip == NULL || port < 0 || port > 65535 || connfd == NULL) 
	{
		ret = Sck_ErrParam;
		printf("func sckClient_getconn() err %d: \
			check (handle == NULL || ip == NULL || port < 0)\n", ret);
		return ret;
	}
	
	tmp = (struct SckHandle *)handle;

	int sockfd;	
	// ��ʼ��socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ret = errno;
		printf("func socket() err %d: malloc\n", ret);
		return ret;
	}
	tmp->sockfd = sockfd;
	
	/*
	// �������ӳ�
	int i = 0;
	for (i = 0; i < nConNum; ++i) 
	{
		if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			ret = errno;
			printf("func socket() err %d: malloc\n", ret);
			return ret;
		}
	}
	*/
	
	struct sockaddr_in srvaddr;
	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(port);
	srvaddr.sin_addr.s_addr = inet_addr(ip);
	
	ret = connect_timeout(sockfd, (struct sockaddr_in *)&srvaddr, tmp->contime);
	if (ret < 0) 
	{
		if (ret == -1 && errno == ETIMEDOUT)
		{
			ret = Sck_ErrTimeOut;
			return ret;
		}
		else
		{
			printf("func connect_timeout() err: %d\n", ret);
		}
	}
	
	*connfd = sockfd;
	
	return ret;
	
}

int sckClient_putconn(void *handle, int *connfd)
{
	int ret = 0;
	SckHandle *tmp = NULL;
	if (handle == NULL) {
		ret = Sck_ErrParam;
		printf("func sckClient_destory() err: %d\n", Sck_ErrParam);
		return ret;
	}
	tmp = (SckHandle*)handle;
	
	close(tmp->sockfd);
	return 0;
}

// �ͻ��˷��ͱ���
int sckClient_send(void *handle, int connfd, char *data, int datalen)
{
	int ret = 0;
	SckHandle *tmp = NULL;
	if (handle == NULL || connfd < 0 || data == NULL) 
	{
		ret = Sck_ErrParam;
		printf("func sckClient_send() err %d: \
			check (handle == NULL || connfd < 0 || data == NULL)\n", ret);
		return ret;
	}
	tmp = (SckHandle*) handle;
	
	ret = write_timeout(connfd, tmp->sendtime);
	if (ret == 0)
	{
		char *netdata = (char *)malloc(datalen + 4);
		if (netdata == NULL) 
		{
			ret = Sck_ErrMalloc;
			printf("func sckClient_send() malloc Err: %d\n", Sck_ErrMalloc);
			return ret;
		}
		int netlen = htonl(datalen);
		// printf("datalen: %d, netlen: %d\n", datalen, netlen);
		memcpy(netdata, &netlen, 4);
		memcpy(netdata+4, data, datalen);
		
		int writed = 0;
		writed = writen(connfd, netdata, datalen + 4);
		if (writed < (datalen + 4))
		{
			if (netdata != NULL)
			{
				free(netdata);
				netdata = NULL;
			}
			return writed;
		}
	}
	else if (ret < 0) 
	{
		if (ret == -1 && errno == ETIMEDOUT) 
		{
			ret = Sck_ErrTimeOut;
			printf("func sckClient_send() Err: %d\n", Sck_ErrTimeOut);
			return ret;
		}
	}
	
	return ret;
}

// �ͻ��˽��ձ���
int sckClient_rev(void *handle, int connfd, char *out, int *outlen)
{
	int ret = 0;
	SckHandle *tmp = NULL;
	if (handle == NULL || connfd < 0 || out == NULL || outlen == NULL) 
	{
		ret = Sck_ErrParam;
		printf("func sckClient_rev() err %d: \
			check (handle == NULL || connfd < 0 || out == NULL || outlen == NULL)\n", ret);
		return ret;
	}
	tmp = (SckHandle*) handle;
	
	// �ɹ���δ��ʱ������0��ʧ�ܷ���-1����ʱ����-1����errno = ETIMEDOUT
	ret = read_timeout(connfd, tmp->revtime);
	if (ret == 0)
	{		
		int readed = 0;
		int len = 0;
		// �ɹ�����count��ʧ�ܷ���-1������EOF����<count
		readed = readn(connfd, &len, 4);
		if (readed == -1) {
			ret = -1;
			printf("func readn() err: %d\n", ret);
			return ret;
		}
		else if (readed < 4) 
		{
			ret = Sck_ErrPeerClosed;
			printf("func readn() err: %d\n", ret);
			return ret;
		}
		len = ntohl(len);
		
		readed = readn(connfd, out, len);
		if (readed == -1) {
			ret = -1;
			printf("func readn() err: %d\n", ret);
			return ret;
		}
		else if (readed < len) 
		{
			ret = Sck_ErrPeerClosed;
			printf("func readn() err: %d\n", ret);
			return ret;
		}
		// printf("readed: %d\n", readed);
		*outlen = len;
	}
	else if (ret < 0) 
	{
		if (ret == -1 && errno == ETIMEDOUT) 
		{
			ret = Sck_ErrTimeOut;
			printf("func sckClient_send() Err: %d\n", Sck_ErrTimeOut);
			return ret;
		}
	}
	
	return ret;
}

// �ͻ��˻����ͷ�
int sckClient_destory(void *handle)
{
	int ret = 0;
	if (handle != NULL) 
	{
		free(handle);
		handle = NULL;
	}
	
	return ret;
}

// --------------------�ͻ���API-------------------

// --------------------��������API-------------------

// �������˻�����ʼ��
int sckServer_init(int port, int *listenfd)
{
	int ret = 0;
	int sockfd = -1;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		ret = errno;
		printf("func socket() error: %d\n", ret);
		return ret;
	}
	
	int on = 1;
	if ((ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) == -1)
	{
		ret = errno;
		printf("func setsockopt() error: %d\n", ret);
		return ret;
	}
	
	if ((ret = bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr))) == -1) 
	{
		ret = errno;
		printf("func bind() error: %d\n", ret);
		return ret;
	}
	
	if ((ret = listen(sockfd, SOMAXCONN)) == -1) 
	{
		ret = errno;
		printf("func listen() error: %d\n", ret);
		return ret;
	}
	
	*listenfd = sockfd;
	
	return 0;
}

// �������˻�ȡ����
int sckServer_accept(int listenfd, int *connfd, int timeout)
{
	int ret = 0;
	/*
	 * �ɹ���δ��ʱ�������������׽��֣���ʱ����-1����errno = ETIMEDOUT
	 */
	ret = accept_timeout(listenfd, NULL, (unsigned int)timeout);
	if (ret < 0)
	{
		if (ret == -1 && errno == ETIMEDOUT) 
		{
			ret = Sck_ErrTimeOut;
			printf("func accept_timeout() err: %d\n", ret);
			return ret;
		}
		else
		{
			ret = errno;
			printf("func accept_timeout() err: %d\n", ret);
			return ret;
		}
	}
	
	*connfd = ret;
	return 0;
}

// �������˷��ͱ���
int sckServer_send(int connfd, char *data, int datalen, int timeout)
{
	int ret = 0;
	
	/*
	* �ɹ���δ��ʱ������0��ʧ�ܷ���-1����ʱ����-1����errno = ETIMEDOUT
	*/
	ret = write_timeout(connfd, (unsigned int)timeout);
	if (ret != 0)
	{
		if (ret == -1 && errno == ETIMEDOUT) 
		{
			ret = Sck_ErrTimeOut;
			printf("func write_timeout() err: %d\n", ret);
			return ret;
		}
		else
		{
			ret = errno;
			printf("func write_timeout() err: %d\n", ret);
			return ret;
		}
	}
	
	if (ret == 0)
	{
		int writed = 0;
		char *netdata = (char*) malloc(datalen + 4);
		if ( netdata == NULL)
		{
			ret = Sck_ErrMalloc;
			printf("func sckServer_send() mlloc Err:%d\n ", ret);
			return ret;
		}
		int netlen = htonl(datalen);
		memcpy(netdata, &netlen, 4);
		memcpy(netdata+4, data, datalen);
		writed = writen(connfd, netdata, datalen+4);
		if (writed < datalen+4)
		{
			if (netdata != NULL) 
			{
				free(netdata);
				netdata = NULL;
			}
			return writed;
		}
		
	}
	
	return 0;
}

// �������˽��ձ���
int sckServer_rev(int connfd, char *out, int *outlen, int timeout)
{
	int ret = 0;
	if (out == NULL || outlen == NULL) 
	{
		ret = Sck_ErrParam;
		printf("func sckServer_rev() param , err:%d \n", ret);
		return ret;
	}
	
	ret = read_timeout(connfd, timeout);
	if (ret != 0)
	{
		if (ret==-1 || errno == ETIMEDOUT)
		{
			ret = Sck_ErrTimeOut;
			printf("func sckServer_rev() timeout , err:%d \n", ret);
			return ret;
		}
		else
		{
			ret = errno;
			printf("func sckServer_rev() , err:%d \n", ret);
			return ret;
		}	
	}
	
	int netdatalen = 0;
	ret = readn(connfd, &netdatalen, 4);
	if (ret == -1) 
	{
		printf("func readn() err:%d \n", ret);
		return ret;
	}
	else if (ret < 4)
	{
		ret = Sck_ErrPeerClosed;
		printf("func readn() err peer closed:%d \n", ret);
		return ret;
	}
	
	int n;
	n = ntohl(netdatalen);
	ret = readn(connfd, out, n);
	if (ret == -1) 
	{
		printf("func readn() err:%d \n", ret);
		return ret;
	}
	else if (ret < n)
	{
		ret = Sck_ErrPeerClosed;
		printf("func readn() err peer closed:%d \n", ret);
		return ret;
	}
	
	*outlen = n;
	
	return 0;
}

// �������˻����ͷ�
int sckServer_destory(int listenfd)
{
	close(listenfd);
	return 0;
}


// --------------------��������API-------------------