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
class Buffer;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    typedef std::shared_ptr<Connection> ptr;
    Connection(const std::shared_ptr<EventLoop>& eventLoop, const std::shared_ptr<Socket>& socket);
    void HandleReadEvent(int fd);
    void SetOnConnectCallback(const std::function<void(Connection::ptr)>& callback);
    void SetDeleteConnectionCallBack(std::function<void(int)> deleteConnectionCallback);

    void ResetReadBuffer(const std::string &readBuffer);
    void SetWriteBuffer(const std::string &writeBuffer);
    std::shared_ptr<Buffer> GetReadBuffer();
    std::shared_ptr<Buffer> GetWriteBuffer();
    void NonBlockRead();
    void Write(); // TODO 是否需要Non block


private:
    std::function<void(std::shared_ptr<Socket>)> m_handleReadEventCallback;
    std::function<void(int)> m_deleteConnectionCallback;
    std::shared_ptr<Channel> m_channel;
    std::shared_ptr<Buffer> m_readBuffer;
    std::shared_ptr<Buffer> m_writeBuffer;
    std::function<void(Connection::ptr)> m_onConnectCallback;
};



#endif //CONNECTION_H
