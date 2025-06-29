//
// Created by liuyu on 6/26/2025.
//

#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#include <vector>

constexpr int32_t MAX_EPOLL_EVENTS = 1024;

class Epoll {
public:
    Epoll();
    ~Epoll();
    std::vector<epoll_event> Poll(int timeout);
    void AddFd(int fd, int events);

private:
    int m_epfd;
    epoll_event *m_events;
};



#endif //EPOLL_H
