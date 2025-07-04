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

#include <arpa/inet.h>

#include "Channel.h"
#include "Logger.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "Socket.h"

#define PORT 8888

constexpr int MAX_EVENTS = 10;

Server::Server(const std::shared_ptr<EventLoop> &eventLoop)
	: m_eventLoop(eventLoop){
	auto socket = std::make_shared<Socket>();
	LOG("Socket created, fd[%d]\n", socket->GetFd());
	socket->SetOpt(SO_REUSEADDR | SO_REUSEPORT);

	auto address = std::make_shared<InetAddress>("127.0.0.1", 8888);
	CHK_PRT(socket->Bind(address) == 0, "Bind failed");
	socket->Listen();
	socket->SetNonBlocking();

	auto channelPtr = std::make_shared<Channel>(m_eventLoop, socket->GetFd());
	std::function<void()> newConnection = [this, socket] { HandleNewConnection(socket); };
	channelPtr->SetHandler(newConnection);
	channelPtr->SetEvents(EPOLLIN | EPOLLET);
	m_eventLoop->UpdateChannel(channelPtr);
	m_channels.push_back(channelPtr);
}

void Server::HandleReadEvent(int fd) {
	LOG("HandleInEvent fd[%d]\n", fd);
	char buf[1024] = { 0 };
	int times = 0;
	while (true) {
		bzero(buf, sizeof(buf));
		int bytesRead = read(fd, buf, sizeof(buf));
		if (bytesRead > 0) {
			times++;
			LOG("read %d bytes from socket, buf:%s\n", bytesRead, buf);

			char sendBuf[1024] = "Hello from Server!\n";
			send(fd, sendBuf, strlen(sendBuf), 0);
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
			close(fd);
			break;
		}
	}
}

void Server::HandleWriteEvent() {
	return ;
}

void Server::HandleNewConnection(std::shared_ptr<Socket> socket) {
	InetAddress::ptr address = std::make_shared<InetAddress>();
	Socket::ptr connSocket = socket->Accept(address);
	LOG("new connection from fd[%d], IP[%s:%d]\n", connSocket->GetFd(),
		inet_ntoa(address->RawSockAddrIn().sin_addr),
		ntohs(address->RawSockAddrIn().sin_port));
	connSocket->SetNonBlocking();
	auto connChannel = std::make_shared<Channel>(m_eventLoop, connSocket->GetFd());
	LOG("connChannel [%d]\n", connChannel->GetFd());
	std::function<void()> readHandler = [this, capture0 = connChannel->GetFd()] { HandleReadEvent(capture0); };
	connChannel->SetHandler(readHandler);
	connChannel->EnableReading();
	m_channels.push_back(connChannel);
}

