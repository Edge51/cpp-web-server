//
// Created by liuyu on 6/26/2025.
//

#include "InetAddress.h"

#include <strings.h>
#include <arpa/inet.h>

InetAddress::InetAddress(const char* addr, unsigned short port)
    : m_address(), m_address_len(sizeof(m_address)){
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = inet_addr(addr);
    m_address.sin_port = htons(port);
    m_address_len = sizeof(m_address);
}

InetAddress::InetAddress() : m_address_len(sizeof(m_address)) {
    bzero(&m_address, sizeof(m_address));
}

sockaddr_in& InetAddress::RawSockAddrIn() {
    return m_address;
}

socklen_t InetAddress::SockLen() const {
    return m_address_len;
}
