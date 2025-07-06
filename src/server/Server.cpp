#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <string>

#include "Server.h"

#include <arpa/inet.h>

#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "Logger.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "Socket.h"

#define PORT 8888

constexpr int MAX_EVENTS = 10;

Server::Server(const std::shared_ptr<EventLoop> &eventLoop)
	: m_eventLoop(eventLoop){
	m_acceptor = std::make_shared<Acceptor>(m_eventLoop);
	std::function<void(std::shared_ptr<Socket>)> acceptHandler = [this](std::shared_ptr<Socket> socket) {
		HandleNewConnection(socket);
	};
	m_acceptor->SetNewConnectionCallback(acceptHandler);
	m_acceptor->EnableAccept();
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
	Connection::ptr connection = std::make_shared<Connection>(m_eventLoop, socket);
	std::function<void(int)> deleteHandler = [this](int fd) {
		DeleteConnection(fd);
	};
	connection->SetDeleteConnectionCallBack(deleteHandler);
	m_connections[socket->GetFd()] = connection;
}

void Server::DeleteConnection(int fd) {
	LOG("erase connection [%d]\n", fd);
	m_connections.erase(fd);
}