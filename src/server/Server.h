#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>

#include "Acceptor.h"

class EventLoop;
class Channel;
class Socket;
class Acceptor;
class Connection;

class Server {
public:
    typedef std::shared_ptr<Server> ptr;
    Server(const std::shared_ptr<EventLoop> &eventLoop);
    void HandleReadEvent(int socketFd);
    void HandleWriteEvent();
    void HandleNewConnection(std::shared_ptr<Socket> socket);
    void DeleteConnection(int fd);
    int ChannelSize() const { return m_channels.size(); }
private:
    std::shared_ptr<EventLoop> m_eventLoop;
    std::shared_ptr<Acceptor> m_acceptor;
    std::vector<std::shared_ptr<Channel>> m_channels;
    std::map<int, std::shared_ptr<Connection>> m_connections;
};

#endif