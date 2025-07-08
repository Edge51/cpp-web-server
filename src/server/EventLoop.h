//
// Created by liuyu on 7/2/2025.
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <functional>
#include <memory>

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
    void UpdateChannel(std::shared_ptr<Channel> channel);
    void AddTask(std::function<void()> task);
private:
    std::shared_ptr<Epoll> m_epoll;
    std::shared_ptr<ThreadPool> m_threadPool;
    bool m_running { false };
};



#endif //EVENTLOOP_H
