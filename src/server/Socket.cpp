#include "Socket.h"

#include <fcntl.h>

#include "Logger.h"
#include <sys/socket.h>

Socket::Socket() {
    m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockFd < 0) {
        LOG("Socket failed");
    }
}

Socket::Socket(int fd) : m_sockFd(fd) {}

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

int Socket::Bind(InetAddress& addr) const {
    sockaddr_in &address = addr.RawSockAddrIn();
    int ret = bind(m_sockFd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
    if (ret < 0) {
        LOG("bind failed");
        return -1;
    }
    LOG("socket binded on port 8888\n");
    return 0;
}

int Socket::Listen() {
    int ret = listen(m_sockFd, 6);
    if (ret < 0) {
        LOG("listen socketFd failed.\n");
        return 1;
    }
    LOG("listening on socketFd:%d\n", m_sockFd);
    return 0;
}

Socket Socket::Accept(InetAddress &addr) {
    socklen_t addrLen = addr.SockLen();
    int connFd = accept(m_sockFd, reinterpret_cast<struct sockaddr *>(&addr.RawSockAddrIn()), &addrLen);
    if (connFd < 0) {
        LOG("accept failed\n");
        return -1;
    }
    LOG("accept connection!!\n");
    return Socket(connFd);
}

int Socket::Connect() {
    return 0;
}