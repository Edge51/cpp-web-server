#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <string>
#include <utility>

#include "Server.h"


#include "Acceptor.h"
#include "Connection.h"
#include "Logger.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"

#define PORT 8888

constexpr int MAX_EVENTS = 10;

Server::Server(const std::shared_ptr<EventLoop> &eventLoop)
	: m_mainReactor(eventLoop){
	m_acceptor = std::make_shared<Acceptor>(m_mainReactor);
	std::function<void(std::shared_ptr<Socket>)> acceptHandler = [this](std::shared_ptr<Socket> socket) {
		HandleNewConnection(std::move(socket));
	};
	m_acceptor->SetNewConnectionCallback(acceptHandler);
	m_acceptor->EnableAccept();

	auto size = std::thread::hardware_concurrency();
	m_threadPool = std::make_shared<ThreadPool>(size);

	for (int i = 0; i < size; i++) {
		m_subReactors.push_back(std::make_shared<EventLoop>());
	}
	for (int i = 0; i < size; i++) {
		m_threadPool->AddTask(std::bind(&EventLoop::Loop, m_subReactors[i]));
	}
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
	int random = socket->GetFd() % m_subReactors.size();
	Connection::ptr connection = std::make_shared<Connection>(m_subReactors[random], socket);
	connection->SetOnConnectCallback(m_onConnect);
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

void Server::SetOnConnect(std::function<void(std::shared_ptr<Connection>)> callback) {
	m_onConnect = callback;
}
