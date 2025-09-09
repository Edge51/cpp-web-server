#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>

#include "TcpConnection.h"


class EventLoop;
class Channel;
class Socket;
class Acceptor;
class TcpConnection;
class ThreadPool;
class EventLoopThreadPool;

class TcpServer {
public:
    typedef std::shared_ptr<TcpServer> ptr;
    TcpServer(const std::shared_ptr<EventLoop> &eventLoop);
    void HandleReadEvent(int socketFd);
    void HandleWriteEvent();
    void HandleNewConnection(std::shared_ptr<Socket> socket);
    void DeleteConnection(int fd);
    int ConnectionCount() { return m_connections.size(); }

    void SetOnConnect(std::function<void(std::shared_ptr<TcpConnection>)> callback);
private:
    std::shared_ptr<EventLoop> m_mainReactor;
    std::vector<std::shared_ptr<EventLoop>> m_subReactors;
    std::shared_ptr<Acceptor> m_acceptor;
    std::map<int, std::shared_ptr<TcpConnection>> m_connections;
    std::shared_ptr<ThreadPool> m_threadPool;
    std::shared_ptr<EventLoopThreadPool> m_eventLoopThreadPool;
    std::function<void(std::shared_ptr<TcpConnection>)> m_onConnect;
};

#endif