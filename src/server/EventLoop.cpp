//
// Created by liuyu on 7/2/2025.
//

#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"

EventLoop::EventLoop() {
    m_epoll = std::make_shared<Epoll>();
}

void EventLoop::Loop() {
    while (m_running) {
        std::vector<Channel *> activeChannels = m_epoll->Poll(-1);
        for (auto channel : activeChannels) {
            channel->HandleEvent();
        }
    }
}

void EventLoop::Start() {
    m_running = true;
    Loop();
}

void EventLoop::Stop() {
    m_running = false;
}

void EventLoop::UpdateChannel(std::shared_ptr<Channel> channel) {
    m_epoll->UpdateChannel(channel);
}

void EventLoop::AddThread(std::function<void()> func) {

}