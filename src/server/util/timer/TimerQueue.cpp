//
// Created by Edge51 on 2025/9/22.
//

#include <sys/timerfd.h>

#include "TimerQueue.h"

#include <cstring>
#include <unistd.h>

#include "Logger.h"

TimerQueue::TimerQueue(std::shared_ptr<EventLoop> loop)
{
    CreateTimerFd();
    m_channel = std::make_shared<Channel>(loop, m_timerFd);
    m_channel->SetHandler([this](){ HandleExpiredTimers();});
    m_channel->EnableReading();
}

void TimerQueue::CreateTimerFd()
{
    m_timerFd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (m_timerFd == -1) {
        LOG("m_timerFd[%d], errno[%d]", m_timerFd, errno);
    }
}

void TimerQueue::ReadTimerFd()
{
    uint64_t readBytes;
    ssize_t bytes = ::read(m_timerFd, &readBytes, sizeof(readBytes));
    if (bytes != sizeof(readBytes)) {
        LOG("read from fd[%d], errno[%d]", m_timerFd, errno);
    }
}

void TimerQueue::HandleExpiredTimers()
{
    ReadTimerFd();
    m_activeTimers.clear();

    auto end = m_timers.upper_bound(Entry(TimeStamp::Now(), std::make_shared<Timer>()));
    m_activeTimers.insert(m_activeTimers.end(), m_timers.begin(), end);
    m_timers.erase(m_timers.begin(), end);

    for (const auto& timer : m_activeTimers) {
        timer.second->Run();
    }
    ResetTimers();
}

bool TimerQueue::InsertTimer(std::shared_ptr<Timer> timer)
{
    bool insert_instantly = false;
    if (m_timers.empty() || timer->ExpirationTime() < m_timers.begin()->first) {
        insert_instantly = true;
    }
    m_timers.emplace(Entry(timer->ExpirationTime(), timer));
    return insert_instantly;
}

void TimerQueue::AddTimer(TimeStamp timestamp, std::function<void()> const &cb, int64_t interval)
{
    auto timer = std::make_shared<Timer>(timestamp, cb, interval);
    if (InsertTimer(timer)) {
        ResetTimerFd(timer);
    }
}

void TimerQueue::ResetTimerFd(std::shared_ptr<Timer> timer)
{
    struct itimerspec newTime;
    struct itimerspec oldTime;
    memset(&newTime, 0, sizeof(newTime));
    memset(&oldTime, 0, sizeof(oldTime));

    int64_t microSecondsDiff = timer->ExpirationTime().MicroSeconds() - TimeStamp::Now().MicroSeconds();
    if (microSecondsDiff < 100) {
        microSecondsDiff = 100;
    }

    newTime.it_value.tv_sec = microSecondsDiff / 1000000;
    newTime.it_value.tv_nsec = (microSecondsDiff % 1000000) * 1000;
    int ret = timerfd_settime(m_timerFd, 0, &newTime, &oldTime);
}

void TimerQueue::ResetTimers()
{
    for (auto iter = m_activeTimers.begin(); iter != m_activeTimers.end(); ) {
        if (iter->second->IsRepeatEnabled()) {
            iter->second->Restart(TimeStamp::Now());
            InsertTimer(iter->second);
            iter++;
        } else {
            m_activeTimers.erase(iter);
        }
    }
    if (!m_timers.empty()) {
        ResetTimerFd(m_timers.begin()->second);
    }
}
