//
// Created by liuyu on 7/2/2025.
//

#include "EventLoop.h"

#include <utility>
#include "Epoll.h"
#include "Channel.h"
#include "timer/TimeStamp.h"

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

void EventLoop::RunAt(TimeStamp timeStamp, const std::function<void()>& cb)
{
    m_timers->AddTimer(timeStamp, cb, 0);
}

void EventLoop::RunAfter(int64_t delayMs, const std::function<void()>& cb)
{
    m_timers->AddTimer(TimeStamp::AddMicroSeconds(TimeStamp::Now(), delayMs), cb, 0);
}

void EventLoop::RunEvery(int64_t intervalMs, const std::function<void()>& cb)
{
    m_timers->AddTimer(TimeStamp::AddMicroSeconds(TimeStamp::Now(), intervalMs), cb, 0);
}

void EventLoop::UpdateChannel(std::shared_ptr<Channel> channel) {
    m_epoll->UpdateChannel(std::move(channel));
}

void EventLoop::AddThread(std::function<void()> func) {

}