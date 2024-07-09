#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "Server.h"

#define PORT 8888


int32_t ServerRun()
{
    printf("Server start.\n");

	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0) {
		printf("Socket failed");
		return -1;
	}

	int opt = 1;
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		printf("Set socket options failed.\n");
		return -1;
	}

	struct sockaddr_in  address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

	int ret = bind(socketFd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
	if (ret < 0) {
		printf("bind failed");
		return 1;
	}
	printf("socket binded on port 8888\n");

	ret = listen(socketFd, 6);
	if (ret < 0) {
		printf("listen socketFd failed.\n");
		return 1;
	}
	printf("listening on socketFd:%d\n", socketFd);

	for (int i = 0; i < 3; i++) {

		printf("------wait for connection the %d time------\n", i + 1);
		socklen_t addressSize = sizeof(address);
		int acceptFd = accept(socketFd, reinterpret_cast<sockaddr*>(&address), &addressSize);
		if (ret < 0) {
			printf("accpet failed\n");
			return 1;
		}
		printf("accept connection!!\n");

		char buf[1024] = { 0 };
		while (true) {
			ret = recvfrom(acceptFd, buf, sizeof(buf), 0, nullptr, nullptr);
			if (ret < 0) {
				printf("recvfrom failed.\n");
				return 1;
			} else if (ret > 0) {
				printf("receive data :%s\n", buf);
				break;
			}
		}
		printf("before send.\n");
		const char *hello = "Hello from Server!\n";
		send(acceptFd, hello, strlen(hello), 0);
		close(acceptFd);
		printf("Sent hello to Client...\n");
		printf("------connection %d end processing------\n", i + 1);
	}

    return 0;
}