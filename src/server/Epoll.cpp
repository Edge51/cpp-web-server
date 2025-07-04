//
// Created by liuyu on 6/26/2025.
//

#include <unistd.h>

#include <strings.h>

#include "Epoll.h"

#include "Channel.h"
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

std::vector<Channel *> Epoll::Poll(int timeout) {
    printf("epoll_wait start\n");
    int nfds = epoll_wait(m_epfd, m_events, MAX_EPOLL_EVENTS, timeout);
    CHK_PRT(nfds == -1, LOG("Poll failed"));
    std::vector<Channel *> activeChannels;
    for (int i = 0; i < nfds; i++) {
        Channel *channel = static_cast<Channel *>(m_events[i].data.ptr);
        channel->SetRevents(m_events[i].events);
        LOG("add channel[%d], &[%p]\n", channel->GetFd(), channel);
        activeChannels.push_back(channel);
    }
    return activeChannels;
}

int Epoll::GetFd() const {
    return m_epfd;
}

void Epoll::AddFd(int fd, int events) {
    epoll_event ev{};
    ev.events = events;
    ev.data.fd = fd;
    CHK_PRT(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == 0, LOG("epoll failed"));
}

void Epoll::UpdateChannel(std::shared_ptr<Channel> channelPtr) {
    const int fd = channelPtr->GetFd();
    const int events = channelPtr->GetEvents();

    epoll_event ev{};
    ev.events = events;
    ev.data.ptr = channelPtr.get();

    if (channelPtr->IsInEpoll()) {
        LOG("epoll_ctl EPOLL_CTL_MOD on ep[%d], fd[%d], events[%d]\n", m_epfd, fd, events);
        CHK_PRT(epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev) == 0, LOG("epoll modify failed"));
    } else {
        LOG("epoll_ctl EPOLL_CTL_ADD on ep[%d], fd[%d], events[%d]\n", m_epfd, fd, events);
        CHK_PRT(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == 0, LOG("epoll add failed"));
        channelPtr->SetInEpoll(true);
    }
}
