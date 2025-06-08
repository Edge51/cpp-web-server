#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>
#include <string>

#include "Server.h"
#include "Logger.h"

#define PORT 8888

constexpr int MAX_EVENTS = 10;

int SetNonBlocking(int sockFd) {
	int attr = fcntl(sockFd, F_GETFL);
	if (attr < 0) {
		LOG("fcntl F_GETFL error");
		return -1;
	}
	return fcntl(sockFd, F_SETFL, attr | O_NONBLOCK);
}

int32_t ServerRun()
{
    LOG("Server start.\n");

	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0) {
		LOG("Socket failed");
		return -1;
	}

	const int opt = 1;
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		LOG("Set socket options failed.\n");
		return -1;
	}

	struct sockaddr_in  address {};
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

	int ret = bind(socketFd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
	if (ret < 0) {
		LOG("bind failed");
		return 1;
	}
	LOG("socket binded on port 8888\n");

	ret = listen(socketFd, 6);
	if (ret < 0) {
		LOG("listen socketFd failed.\n");
		return 1;
	}
	LOG("listening on socketFd:%d\n", socketFd);

	int epollFd = epoll_create1(0);
	struct epoll_event event = {};
	event.data.fd = socketFd;
	event.events = EPOLLIN;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &event) == -1) {
		LOG("epoll_ctl failed\n");
		return -1;
	}

	struct epoll_event events[MAX_EVENTS];
	while (true) {
		int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			LOG("epoll_wait failed\n");
			return -1;
		}

		int times = 0;
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == socketFd) {
				socklen_t addressSize = sizeof(address);
				int connFd = accept(socketFd, reinterpret_cast<struct sockaddr *>(&address), &addressSize);
				if (connFd < 0) {
					LOG("accept failed\n");
					return -1;
				}
				LOG("accept connection!!\n");
				SetNonBlocking(connFd);
				event.data.fd = connFd;
				event.events = EPOLLIN | EPOLLET;
				if (epoll_ctl(epollFd, EPOLL_CTL_ADD, connFd, &event) == -1) {
					LOG("epoll_ctl failed\n");
					return -1;
				}
			} else if (events[i].events & EPOLLIN) {
				char buf[1024] = { 0 };
				while (true) {
					bzero(buf, sizeof(buf));
					int bytesRead = read(events[i].data.fd, buf, sizeof(buf));
					if (bytesRead > 0) {
						times++;
						LOG("read %d bytes from socket, buf:%s\n", bytesRead, buf);

						char sendBuf[1024] = "Hello from Server!\n";
						send(events[i].data.fd, sendBuf, strlen(sendBuf), 0);
						if (times == 3) {
							LOG("receive end\n");
							return 0;
						}
					} else if (bytesRead == -1 && errno == EINTR) {
						LOG("errno[%d] EINTR, means client interupted, continue recv\n", errno);
						continue;
					} else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
						LOG("errno[%d] EAGAIN OR EWOULDBLOCK, means recv finish\n", errno);
						break;
					} else if (bytesRead == 0) {
						LOG("bytesRead[%d], client disconnected\n", bytesRead);
						close(events[i].data.fd);
						break;
					}
				}
			} else {
				LOG("Somthing else\n");
			}
		}
	}
    return 0;
}