#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>

#include "Connection.h"


class EventLoop;
class Channel;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;

class Server {
public:
    typedef std::shared_ptr<Server> ptr;
    Server(const std::shared_ptr<EventLoop> &eventLoop);
    void HandleReadEvent(int socketFd);
    void HandleWriteEvent();
    void HandleNewConnection(std::shared_ptr<Socket> socket);
    void DeleteConnection(int fd);
    int ConnectionCount() { return m_connections.size(); }

    void SetOnConnect(std::function<void(std::shared_ptr<Connection>)> callback);
private:
    std::shared_ptr<EventLoop> m_mainReactor;
    std::vector<std::shared_ptr<EventLoop>> m_subReactors;
    std::shared_ptr<Acceptor> m_acceptor;
    std::map<int, std::shared_ptr<Connection>> m_connections;
    std::shared_ptr<ThreadPool> m_threadPool;
    std::function<void(std::shared_ptr<Connection>)> m_onConnect;
};

#endif