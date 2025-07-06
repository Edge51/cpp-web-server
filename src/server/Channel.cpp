//
// Created by liuyu on 6/29/2025.
//

#include <unistd.h>

#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "Logger.h"
#include "Socket.h"

Channel::Channel(const EventLoop::ptr& eventLoop, Socket::ptr socket)
    : m_eventLoop(eventLoop), m_socket(socket) {
}

Channel::~Channel() {
}

void Channel::EnableReading() {
    m_events = EPOLLIN | EPOLLET;
    m_eventLoop->UpdateChannel(std::make_shared<Channel>(*this));
}

void Channel::HandleEvent() {
    m_handler();
}

void Channel::SetHandler(std::function<void()> handler) {
    LOG("Channel::SetHandler\n");
    m_handler = handler;
}

int Channel::GetFd() const {
    return m_socket->GetFd();
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
