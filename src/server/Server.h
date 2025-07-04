#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <memory>
#include <vector>

class EventLoop;
class Channel;
class Socket;

class Server {
public:
    typedef std::shared_ptr<Server> ptr;
    Server(const std::shared_ptr<EventLoop> &eventLoop);
    void HandleReadEvent(int socketFd);
    void HandleWriteEvent();
    void HandleNewConnection(std::shared_ptr<Socket> socket);
    int ChannelSize() const { return m_channels.size(); }
private:
    std::shared_ptr<EventLoop> m_eventLoop;
    std::vector<std::shared_ptr<Channel>> m_channels;
};

#endif