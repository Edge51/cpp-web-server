#ifndef BIND_SOCKET_H
#define BIND_SOCKET_H

#include <memory>

#include "InetAddress.h"

class Socket {
public:
    typedef std::shared_ptr<Socket> ptr;
    Socket();
    Socket(int fd);
    int GetFd() const;
    int SetOpt(int opName);
    int SetNonBlocking();
    int Bind(InetAddress::ptr addr) const;
    int Listen();
    Socket::ptr Accept(InetAddress::ptr addr);
    int Connect();
private:
    int m_sockFd;
};

#endif