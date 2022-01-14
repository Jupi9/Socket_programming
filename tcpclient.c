#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Help: %s [ip|host] [port]\n", argv[0]); return 1;
	}
	WSADATA w;
	int ret = WSAStartup(0x0020002, &w);
	if (ret != 0) {
		printf("Winsock API not ready\n"); return 1;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == SOCKET_ERROR) {
		return 1;
	}
	SOCKADDR_IN sa;		//prepare server address
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[2]));
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	ret = connect(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) {		//there are many many reasons for this error
		printf("Connection failed, please check your Internet\n"); return 1;
	}
	printf("Server %s is connected\n", argv[1]);
	//while (1) {
	//char sbuf[10000] = "GET /index.html HTTP/1.1\r\nHost: 192.168.73.144\r\nAccept: text/html\r\n\r\n";
	//ret = send(s, sbuf, strlen(sbuf), 0);
	char sbuf[10000];
	memset(sbuf, 0, 10000);
	
	ret = recv(s, sbuf, 10000, 0);
	while (ret > 0) {
		printf("Server :: %s\n", sbuf);
		memset(sbuf, 0, 10000);
		ret = recv(s, sbuf, 10000, 0);
	}
	//}
	closesocket(s);
	WSACleanup();
	printf("TCP client is closed\n");
}