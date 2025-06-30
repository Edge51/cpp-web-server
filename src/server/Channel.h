//
// Created by liuyu on 6/29/2025.
//

#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
#include <sys/epoll.h>

class Epoll;

class Channel {
public:
    typedef std::shared_ptr<Channel> ptr;

    Channel(const std::shared_ptr<Epoll>& ep, int fd);
    ~Channel();
    void EnableReading();
    int GetFd() const;
    void SetEvents(int events);
    int GetEvents() const;
    void SetRevents(int revents);
    int GetRevents() const;

    bool IsInEpoll() const;
    void SetInEpoll(bool isInEpoll);
private:
    std::shared_ptr<Epoll> m_epoll;
    int m_fd;
    uint32_t m_events;
    uint32_t m_revents;
    bool m_inEpoll { false };
};



#endif //CHANNEL_H
