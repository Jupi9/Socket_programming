#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>

int main(int argc, char** argv) {

	if (argc != 2) {
		printf("Help: %s [port]\n", argv[0]); return 1;
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

	SOCKADDR_IN sa, ca;		//prepare server/local address
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[1]));
	sa.sin_addr.s_addr = INADDR_ANY;
	ret = bind(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) {
		printf("Bind failed, please check the port\n");
		return 1;
	}

	ret = listen(s, 5);
	if (ret == SOCKET_ERROR) {
		return 1;
	}

	printf("Server is listening\n");
	while (1) {
		
		SOCKET cs = SOCKET_ERROR;
		int calen = sizeof(ca);

		while (cs == SOCKET_ERROR) {
			cs = accept(s, (SOCKADDR*)&ca, &calen);
		}

		printf("Client %s:%d logged in\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
		//Now we can communicate with that client, if this is a wrong guy
		char sbuf[100] = "This is not the right guy you can play with \r\n";
		for (int i = 0; i < 100; i++) {
			ret = send(cs, sbuf, strlen(sbuf), 0);
		}
		closesocket(cs);
	}
}
