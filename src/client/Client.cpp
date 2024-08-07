#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#define PORT 8888


#include "Client.h"

std::string ClientRequest()
{
    printf("Client start.\n");

	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0) {
		printf("socket failed\n");
		return "";
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	
	int ret = inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	if (ret	!= 1) {
		printf("inet_pton failed, with error code:%d\n", ret);
		return "";
	}

	ret = connect(socketFd, reinterpret_cast<sockaddr *>(&servAddr), sizeof(servAddr));
	if (ret < 0) {
		printf("connect failed:%s\n", strerror(errno));
		return "";
	}

	printf("Before Client send...\n");
	const char *hello = "hello from client";
	if (send(socketFd, hello, strlen(hello), 0) < 0) {
		printf("send failed with:%s\n", strerror(errno));
	}

	printf("Sent Hello to Server.\n");
	char buffer[1024] = { 0 };
	int readRet = read(socketFd, buffer, 1024);
	printf("Receive message:%s\n", buffer);

	close(socketFd);

	return buffer;
}