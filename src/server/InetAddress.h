//
// Created by liuyu on 6/26/2025.
//

#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <netinet/in.h>

class InetAddress {
public:
    explicit InetAddress(const char* addr, unsigned short port = 0);
    sockaddr_in& RawSockAddrIn();
    socklen_t SockLen() const;
private:
    sockaddr_in m_address;
    socklen_t m_address_len;
};



#endif //INETADDRESS_H
