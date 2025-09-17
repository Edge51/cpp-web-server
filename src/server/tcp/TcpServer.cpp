#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <string>
#include <utility>

#include "TcpServer.h"


#include "Acceptor.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "EventLoop.h"
#include "Socket.h"
#include "EventLoopThreadPool.h"

#define PORT 8888

constexpr int MAX_EVENTS = 10;

TcpServer::TcpServer(const std::shared_ptr<EventLoop> &eventLoop)
	: m_mainReactor(eventLoop){
	m_acceptor = std::make_shared<Acceptor>(m_mainReactor);
	std::function<void(std::shared_ptr<Socket>)> acceptHandler = [this](std::shared_ptr<Socket> socket) {
		HandleNewConnection(std::move(socket));
	};
	m_acceptor->SetNewConnectionCallback(acceptHandler);
	m_acceptor->EnableAccept();

	auto size = std::thread::hardware_concurrency();

	m_eventLoopThreadPool = std::make_shared<EventLoopThreadPool>(size);
}

void TcpServer::HandleNewConnection(std::shared_ptr<Socket> socket) {
	TcpConnection::ptr connection = std::make_shared<TcpConnection>(m_eventLoopThreadPool->GetNextLoop(), socket);
	connection->SetOnConnectCallback(m_onConnect);
	std::function<void(int)> deleteHandler = [this](int fd) {
		DeleteConnection(fd);
	};
	connection->SetDeleteConnectionCallBack(deleteHandler);
	m_connections[socket->GetFd()] = connection;
}

void TcpServer::DeleteConnection(int fd) {
	LOG("erase connection [%d]\n", fd);
	m_connections.erase(fd);
}

void TcpServer::SetOnConnect(std::function<void(std::shared_ptr<TcpConnection>)> callback) {
	m_onConnect = callback;
}
