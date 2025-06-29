//
// Created by liuyu on 6/26/2025.
//

#include <unistd.h>

#include <strings.h>

#include "Epoll.h"
#include "Logger.h"

Epoll::Epoll() {
    m_epfd = epoll_create1(0);
    CHK_PRT(m_epfd == -1, "epoll_create1 failed");
    m_events = new epoll_event[MAX_EPOLL_EVENTS];
    bzero(m_events, sizeof(epoll_event) * MAX_EPOLL_EVENTS);
}

Epoll::~Epoll() {
    if (m_epfd != -1) {
        close(m_epfd);
    }
    if (m_events) {
        delete[] m_events;
    }
}

std::vector<epoll_event> Epoll::Poll(int timeout) {
    int nfds = epoll_wait(m_epfd, m_events, MAX_EPOLL_EVENTS, timeout);
    CHK_PRT(nfds == -1, LOG("Poll failed"));
    std::vector<epoll_event> activeEvent;
    for (int i = 0; i < nfds; i++) {
        activeEvent.push_back(m_events[i]);
    }
    return activeEvent;
}

void Epoll::AddFd(int fd, int events) {
    epoll_event ev{};
    ev.events = events;
    ev.data.fd = fd;
    CHK_PRT(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == 0, LOG("epoll failed"));
}
