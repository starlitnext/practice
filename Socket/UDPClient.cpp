#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		return 0;
	}
	SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int len = sizeof(sin);

	char * sendData = "来自客户端的数据包.\n";
	sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

	string sayToServer;
	while (cin >> sayToServer)
	{
		const char *data = sayToServer.c_str();
		sendto(sclient, data, strlen(data), 0, (sockaddr *)&sin, sizeof(sin));
	}

	char recvData[255];
	int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
	if (ret > 0)
	{
		recvData[ret] = 0x00;
		printf(recvData);
	}

	closesocket(sclient);
	WSACleanup();

	system("pause");
	return 0;
}