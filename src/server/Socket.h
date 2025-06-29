#ifndef BIND_SOCKET_H
#define BIND_SOCKET_H

#include "InetAddress.h"

class Socket {
public:
    Socket();
    Socket(int fd);
    int GetFd() const;
    int SetOpt(int opName);
    int SetNonBlocking();
    int Bind(InetAddress &addr) const;
    int Listen();
    Socket Accept(InetAddress &addr);
    int Connect();
private:
    int m_sockFd;
};

#endif