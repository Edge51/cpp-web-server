#include "Socket.h"
#include <sys/socket.h>

Socket::Socket() {

}

int Socket::SetOpt(int opName) {
    const int opt = 1;
    if (setsockopt(m_sockFd, SOL_SOCKET, opName, &opt, sizeof(opt))) {
        return -1;
    }
    return 0;
}

int Socket::Bind(int port) {

}

int Socket::Listen(int backlog) {

}

int Socket::Accept() {
}

int Socket::Connect() {
}