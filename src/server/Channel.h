//
// Created by liuyu on 6/29/2025.
//

#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <memory>
#include <sys/epoll.h>

class EventLoop;
class Socket;

class Channel {
public:
    typedef std::shared_ptr<Channel> ptr;

    Channel(const std::shared_ptr<EventLoop>& ep, std::shared_ptr<Socket> socket);
    ~Channel();
    void EnableReading();
    void HandleEvent();
    void SetHandler(std::function<void()> handler);

    int GetFd() const;
    void SetEvents(int events);
    int GetEvents() const;
    void SetRevents(int revents);
    int GetRevents() const;

    bool IsInEpoll() const;
    void SetInEpoll(bool isInEpoll);
private:
    std::shared_ptr<EventLoop> m_eventLoop;
    std::shared_ptr<Socket> m_socket;
    std::function<void()> m_handler;
    uint32_t m_events;
    uint32_t m_revents;
    bool m_inEpoll { false };
};



#endif //CHANNEL_H
