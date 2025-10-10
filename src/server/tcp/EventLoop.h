//
// Created by liuyu on 7/2/2025.
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <functional>
#include <memory>

#include "timer/TimerQueue.h"
#include "timer/TimeStamp.h"

class Epoll;
class ThreadPool;
class Channel;

class EventLoop {
public:
    using ptr = std::shared_ptr<EventLoop>;
    explicit EventLoop();
    void Loop();
    void Start();
    void Stop();

    void RunAt(TimeStamp timeStamp, const std::function<void()>& cb);
    void RunAfter(int64_t delayMs, const std::function<void()>& cb);
    void RunEvery(int64_t intervalMs, const std::function<void()>& cb);

    void UpdateChannel(std::shared_ptr<Channel> channel);
    void AddThread(std::function<void()> func);
private:
    std::shared_ptr<Epoll> m_epoll;
    bool m_running { true };

    std::shared_ptr<TimerQueue> m_timers;
};



#endif //EVENTLOOP_H
