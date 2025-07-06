//
// Created by liuyu on 7/6/2025.
//

#ifndef CONNECTION_H
#define CONNECTION_H
#include <memory>
#include <functional>

class Socket;
class Channel;
class EventLoop;

class Connection {
public:
    typedef std::shared_ptr<Connection> ptr;
    Connection(const std::shared_ptr<EventLoop>& eventLoop, const std::shared_ptr<Socket>& socket);
    void HandleReadEvent(int fd);
    void SetDeleteConnectionCallBack(std::function<void(int)> deleteConnectionCallback);
private:
    std::function<void(std::shared_ptr<Socket>)> m_handleReadEventCallback;
    std::function<void(int)> m_deleteConnectionCallback;
    std::shared_ptr<Channel> m_channel;
};



#endif //CONNECTION_H
