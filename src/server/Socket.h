#ifndef BIND_SOCKET_H
#define BIND_SOCKET_H

class Socket {
public:
    Socket();
    int SetOpt(int opName);
    int Bind(int port);
    int Listen(int backlog);
    int Accept();
    int Connect();
private:
    int m_sockFd;
};

#endif