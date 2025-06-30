//
// Created by liuyu on 6/29/2025.
//

#include "Channel.h"
#include "Epoll.h"
#include "Logger.h"

Channel::Channel(const Epoll::ptr& ep, int fd) : m_epoll(ep), m_fd(fd) {
}

Channel::~Channel() {
    LOG("~Channel\n");
    m_epoll = nullptr;
    m_fd = -1;
}

void Channel::EnableReading() {
    m_events = EPOLLIN | EPOLLET;
    m_epoll->UpdateChannel(this);
}

int Channel::GetFd() const {
    return m_fd;
}

void Channel::SetEvents(int events) {
    m_events = events;
}

int Channel::GetEvents() const {
    return m_events;
}

void Channel::SetRevents(int revents) {
    m_revents = revents;
}

int Channel::GetRevents() const {
    return m_revents;
}

bool Channel::IsInEpoll() const {
    return m_inEpoll;
}

void Channel::SetInEpoll(bool isInEpoll) {
    LOG("inEpoll set to %s\n", isInEpoll ? "true" : "false");
    m_inEpoll = isInEpoll;
}
