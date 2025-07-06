//
// Created by liuyu on 7/6/2025.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <cerrno>
#include <utility>

#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include "Buffer.h"

Connection::Connection(const EventLoop::ptr& eventLoop, const Socket::ptr& socket) {
    m_channel = std::make_shared<Channel>(eventLoop, socket);
    std::function<void()> connectCallback = [this]() { HandleReadEvent(m_channel->GetFd());};
    m_channel->SetHandler(connectCallback);
    m_channel->SetEvents(EPOLLIN | EPOLLET);
    eventLoop->UpdateChannel(m_channel);
    m_readBuffer = std::make_shared<Buffer>();
    m_writeBuffer = std::make_shared<Buffer>();
}

void Connection::HandleReadEvent(int fd) {
    LOG("HandleInEvent fd[%d]\n", fd);
    char buf[1024] = { 0 };
    while (true) {
        bzero(buf, sizeof(buf));
        int bytesRead = read(fd, buf, sizeof(buf));
        if (bytesRead > 0) {
            LOG("read %d bytes from socket, buf:%s\n", bytesRead, buf);
            m_readBuffer->Append(buf, bytesRead);
        } else if (bytesRead == -1 && errno == EINTR) {
            LOG("errno[%d] EINTR, means client interupted, continue recv\n", errno);
            continue;
        } else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            LOG("errno[%d] EAGAIN OR EWOULDBLOCK, means recv finish\n", errno);
            LOG("readBuffer[%s]\n", m_readBuffer->c_str());
            char sendBuf[1024] = "Hello from Server!\n";
            m_writeBuffer->Append(sendBuf, sizeof(sendBuf));
            send(fd, m_writeBuffer->c_str(), m_writeBuffer->Size(), 0);
            m_readBuffer->Clear();
            m_writeBuffer->Clear();
            break;
        } else if (bytesRead == 0) {
            LOG("bytesRead[%d], client disconnected\n", bytesRead);
            m_deleteConnectionCallback(m_channel->GetFd());
            break;
        }
    }
}

void Connection::SetDeleteConnectionCallBack(std::function<void(int)> deleteConnectionCallback) {
    m_deleteConnectionCallback = std::move(deleteConnectionCallback);
}