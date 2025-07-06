//
// Created by liuyu on 7/4/2025.
//

#include "Acceptor.h"

#include <arpa/inet.h>

#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"

Acceptor::Acceptor(EventLoop::ptr eventLoop) : m_eventLoop(eventLoop) {
    m_socket = std::make_shared<Socket>();
    LOG("Socket created, fd[%d]\n", m_socket->GetFd());
    m_socket->SetOpt(SO_REUSEADDR | SO_REUSEPORT);

    auto address = std::make_shared<InetAddress>("127.0.0.1", 8888);
    CHK_PRT(m_socket->Bind(address) == 0, "Bind failed");
    m_socket->Listen();
    m_socket->SetNonBlocking();

    m_channel = std::make_shared<Channel>(m_eventLoop, m_socket);
}

Acceptor::~Acceptor() {
}

void Acceptor::SetNewConnectionCallback(std::function<void(std::shared_ptr<Socket>)> callback) {
    LOG("SetNewConnectionCallback\n");
    m_onNewConnection = callback;
    std::function<void()> newConnection = [this] { HandleNewConnection(); };
    m_channel->SetHandler(newConnection);
}

void Acceptor::HandleNewConnection() {
    InetAddress::ptr address = std::make_shared<InetAddress>();
    Socket::ptr connSocket = m_socket->Accept(address);
    LOG("new connection from fd[%d], IP[%s:%d]\n", connSocket->GetFd(),
        inet_ntoa(address->RawSockAddrIn().sin_addr),
        ntohs(address->RawSockAddrIn().sin_port));
    connSocket->SetNonBlocking();
    m_onNewConnection(connSocket);
}

Channel::ptr Acceptor::GetChannel() {
    return m_channel;
}

void Acceptor::EnableAccept() {
    LOG("EnableAccept\n");
    m_channel->SetEvents(EPOLLIN | EPOLLET);
    m_eventLoop->UpdateChannel(m_channel);
}