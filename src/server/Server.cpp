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
#include "Epoll.h"
#include "Socket.h"

#define PORT 8888

constexpr int MAX_EVENTS = 10;



void HandleInEvent(epoll_event activeEvent) {
	char buf[1024] = { 0 };
	int times = 0;
	while (true) {
		bzero(buf, sizeof(buf));
		int bytesRead = read(activeEvent.data.fd, buf, sizeof(buf));
		if (bytesRead > 0) {
			times++;
			LOG("read %d bytes from socket, buf:%s\n", bytesRead, buf);

			char sendBuf[1024] = "Hello from Server!\n";
			send(activeEvent.data.fd, sendBuf, strlen(sendBuf), 0);
			if (times == 3) {
				LOG("receive end\n");
				return ;
			}
		} else if (bytesRead == -1 && errno == EINTR) {
			LOG("errno[%d] EINTR, means client interupted, continue recv\n", errno);
			continue;
		} else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
			LOG("errno[%d] EAGAIN OR EWOULDBLOCK, means recv finish\n", errno);
			break;
		} else if (bytesRead == 0) {
			LOG("bytesRead[%d], client disconnected\n", bytesRead);
			close(activeEvent.data.fd);
			break;
		}
	}
}

int32_t ServerRun()
{
    LOG("Server start.\n");
	Socket socket;
	socket.SetOpt(SO_REUSEADDR | SO_REUSEPORT);

	InetAddress address("127.0.0.1", 8888);
	socket.Bind(address);
	socket.Listen();

	Epoll epoll;
	epoll.AddFd(socket.GetFd(), EPOLLIN | EPOLLET);

	while (true) {
		std::vector<epoll_event> activeEvents = epoll.Poll(-1);
		for (auto& activeEvent : activeEvents) {
			if (activeEvent.data.fd == socket.GetFd()) {
				socklen_t addressSize = sizeof(address);
				Socket connSocket = socket.Accept(address);
				connSocket.SetNonBlocking();
				epoll.AddFd(connSocket.GetFd(), EPOLLIN | EPOLLET);
			} else if (activeEvent.events & EPOLLIN) {
				HandleInEvent(activeEvent);
			} else {
				LOG("Somthing else\n");
			}
		}
	}
    return 0;
}