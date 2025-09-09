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

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    typedef std::shared_ptr<TcpConnection> ptr;
    TcpConnection(const std::shared_ptr<EventLoop>& eventLoop, const std::shared_ptr<Socket>& socket);
    void HandleReadEvent(int fd);
    void SetOnConnectCallback(const std::function<void(TcpConnection::ptr)>& callback);
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
    std::function<void(TcpConnection::ptr)> m_onConnectCallback;
    
    std::shared_ptr<Channel> m_channel;
    std::shared_ptr<Buffer> m_readBuffer;
    std::shared_ptr<Buffer> m_writeBuffer;
};



#endif //CONNECTION_H
