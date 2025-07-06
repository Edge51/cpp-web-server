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

Connection::Connection(const EventLoop::ptr& eventLoop, const Socket::ptr& socket) {
    m_channel = std::make_shared<Channel>(eventLoop, socket);
    std::function<void()> connectCallback = [this]() { HandleReadEvent(m_channel->GetFd());};
    m_channel->SetHandler(connectCallback);
    m_channel->SetEvents(EPOLLIN | EPOLLET);
    eventLoop->UpdateChannel(m_channel);
}

void Connection::HandleReadEvent(int fd) {
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
            m_deleteConnectionCallback(m_channel->GetFd());
            break;
        }
    }
}

void Connection::SetDeleteConnectionCallBack(std::function<void(int)> deleteConnectionCallback) {
    m_deleteConnectionCallback = std::move(deleteConnectionCallback);
}