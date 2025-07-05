//
// Created by liuyu on 7/4/2025.
//

#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <memory>
#include <functional>

class Socket;
class EventLoop;
class Channel;

class Acceptor {
public:
    typedef std::shared_ptr<Acceptor> ptr;
    Acceptor(std::shared_ptr<EventLoop> eventLoop);
    ~Acceptor();

    void HandleNewConnection();
    void EnableAccept();
    void SetNewConnectionCallback(std::function<void(std::shared_ptr<Socket>)> callback);
    std::shared_ptr<Channel> GetChannel();
private:
    std::shared_ptr<Socket> m_socket;
    std::shared_ptr<EventLoop> m_eventLoop;
    std::shared_ptr<Channel> m_channel;
    std::function<void(std::shared_ptr<Socket>)> m_onNewConnection;
};

#endif //ACCEPTOR_H
