//
// Created by Edge51 on 2025/9/22.
//

#ifndef CPPWEBSERVER_TIMERQUEUE_H
#define CPPWEBSERVER_TIMERQUEUE_H

#include <memory>
#include <set>
#include <utility>

#include "Channel.h"
#include "Timer.h"

class EventLoop;

class TimerQueue {
public:
    TimerQueue(std::shared_ptr<EventLoop> loop);
public:
    void CreateTimerFd();

    void ReadTimerFd();
    void HandleExpiredTimers();

    bool InsertTimer(std::shared_ptr<Timer> timer);
    void AddTimer(TimeStamp timestamp, std::function<void()> const &cb, int64_t interval);

    void ResetTimerFd(std::shared_ptr<Timer> timer);
    void ResetTimers();
private:
    using Entry = std::pair<TimeStamp, std::shared_ptr<Timer>>;
    std::shared_ptr<Channel> m_channel;
    std::shared_ptr<EventLoop> m_eventLoop;
    int m_timerFd;
    std::set<Entry> m_timers;
    std::vector<Entry> m_activeTimers;
};


#endif //CPPWEBSERVER_TIMERQUEUE_H