#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define PORT 8888

int main(int argc, char* argv[])
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
	address.sin_port = PORT;

	int ret = bind(socketFd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
	if (ret < 0) {
		printf("bind failed");
		return 1;
	}

	ret = listen(socketFd, 6);
	if (ret < 0) {
		printf("listen socketFd failed.\n");
		return 1;
	}

	socklen_t addressSize = sizeof(address);
	int acceptFd = accept(socketFd, reinterpret_cast<sockaddr*>(&address), &addressSize);
	if (ret < 0) {
		printf("accpet failed");
		return 1;
	}

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
	printf("Sent hello to Client...\n");

	return 0;
}