//
// Created by liuyu on 7/2/2025.
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Epoll.h"

class EventLoop {
public:
    typedef std::shared_ptr<EventLoop> ptr;
    explicit EventLoop();
    void Loop();
    void Start();
    void Stop();
    void UpdateChannel(std::shared_ptr<Channel> channel);
private:
    Epoll::ptr m_epoll;
    bool m_running { false };
};



#endif //EVENTLOOP_H
