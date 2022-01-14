#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <Winsock2.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {	// command "ip" "port"
		printf("Help: %s [IP] [Port]\n", argv[0]);
		return 1;
	}
	WSADATA w;
	int ret = WSAStartup(MAKEWORD(2,2), &w);
	if (ret != 0) {
		printf("Winsock API cannot be started\n");
		return 1;
	}
	printf("Winsock API ready\n");

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (s == SOCKET_ERROR) {
		printf("Socket cannot be created\n");
		return 1;
	}
	
	//Prepare remote (server) address
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	sa.sin_port = htons(atoi(argv[2]));
	
	printf("UDP client is communicating with %s:%s\n", argv[1], argv[2]);
	
	while (1) {
		char sbuf[100];

		printf("Message>> ");
		fgets(sbuf, 100, stdin);	//is the user types "EXIT", break the loop
		int slen = strlen(sbuf);
		sbuf[slen - 1] = '\0';

		if (strcmp(sbuf, "exit") == 0 || strcmp(sbuf, "EXIT") == 0) {
			printf("Goodbye\n");
			break;
		}
		ret = sendto(s, sbuf, strlen(sbuf), 0, (SOCKADDR *)&sa, sizeof(sa));
		//recvfrom if the server sends something back
		int fromlen = sizeof(sa);
		memset(sbuf, 0, 100);
		ret = recvfrom(s, sbuf, 100, 0, (SOCKADDR*)&sa, sizeof(sa));
		if (ret > 0) {
			printf("Server: %s\n", sbuf);
		}
	}
	closesocket(s);	//Destroy the socket
	WSACleanup();	//Stop Winsock API
}
