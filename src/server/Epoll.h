//
// Created by liuyu on 6/26/2025.
//

#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#include <vector>
#include <memory>

constexpr int32_t MAX_EPOLL_EVENTS = 1024;

class Channel;

class Epoll {
public:
    typedef std::shared_ptr<Epoll> ptr;
    Epoll();
    ~Epoll();
    std::vector<Channel *> Poll(int timeout);
    int GetFd() const;
    void AddFd(int fd, int events);
    void UpdateChannel(std::shared_ptr<Channel> channelPtr);

private:
    int m_epfd;
    epoll_event *m_events;
};



#endif //EPOLL_H
