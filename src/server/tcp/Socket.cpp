#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#include "Logger.h"
#include "Socket.h"

Socket::Socket() {
    m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockFd < 0) {
        LOG("Socket failed");
    }
}

Socket::Socket(int fd) : m_sockFd(fd) {}

Socket::~Socket() {
    LOG("close socket, fd[%d]", m_sockFd);
    if (m_sockFd >= 0) {
        close(m_sockFd);
        m_sockFd = -1;
    }
}

int Socket::GetFd() const {
    return m_sockFd;
}

int Socket::SetOpt(int opName) {
    const int opt = 1;
    if (setsockopt(m_sockFd, SOL_SOCKET, opName, &opt, sizeof(opt))) {
        return -1;
    }
    return 0;
}

int Socket::SetNonBlocking() {
    int attr = fcntl(m_sockFd, F_GETFL);
    if (attr < 0) {
        LOG("fcntl F_GETFL error");
        return -1;
    }
    return fcntl(m_sockFd, F_SETFL, attr | O_NONBLOCK);
}

int Socket::Bind(InetAddress::ptr addr) const {
    sockaddr_in &address = addr->RawSockAddrIn();
    int ret = bind(m_sockFd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
    if (ret < 0) {
        LOG("bind failed");
        return -1;
    }
    LOG("socket binded on port %s:%d", inet_ntoa(addr->RawSockAddrIn().sin_addr),
        ntohs(addr->RawSockAddrIn().sin_port));
    return 0;
}

int Socket::Listen() {
    int ret = listen(m_sockFd, 6);
    if (ret < 0) {
        LOG("listen socketFd failed.\n");
        return 1;
    }
    LOG("listening on socketFd:%d", m_sockFd);
    return 0;
}

Socket::ptr Socket::Accept(InetAddress::ptr addr) {
    socklen_t addrLen = addr->SockLen();
    int connFd = accept(m_sockFd, reinterpret_cast<struct sockaddr *>(addr.get()), &addrLen);
    if (connFd < 0) {
        LOG("accept failed\n");
        return nullptr;
    }
    LOG("accept connection!! fd[%d]\n", connFd);
    return std::make_shared<Socket>(connFd);
}

int Socket::Connect() {
    return 0;
}