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
    void AddThread(std::function<void()> func);
private:
    std::shared_ptr<Epoll> m_epoll;
    bool m_running { true };
};



#endif //EVENTLOOP_H
