#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>


// gethostname function has to be used in most client programs, because the user
// usually uses hostname instead of ip address
int main() {
	WSADATA w;
	int ret = WSAStartup(0x0002002, &w);
	
	if (ret != 0) {
		printf("winsock API cannot start\n");
	}

	printf("I am going to translate hostname into ip address\n");

	while (1) {
		printf("Host name >> ");
		char hname[100];
		scanf("%s", hname);

		if (strcmp(hname, "EXIT") == 0) break;
		
		LPHOSTENT lph = gethostbyname(hname);
		SOCKADDR_IN sa;
		sa.sin_addr = *((LPIN_ADDR)*lph->h_addr_list);
		printf("%s has IP address as %s\n", hname, inet_ntoa(sa.sin_addr) );

	}
	
	WSACleanup();
	printf("Program stops\n");
}

