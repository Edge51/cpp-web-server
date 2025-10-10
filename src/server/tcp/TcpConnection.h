//
// Created by liuyu on 7/6/2025.
//

#ifndef CONNECTION_H
#define CONNECTION_H
#include <memory>
#include <functional>

#include "timer/TimeStamp.h"

class Socket;
class Channel;
class EventLoop;
class Buffer;
namespace http {
class HttpRequestParser;
} // namespace http

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    typedef std::shared_ptr<TcpConnection> ptr;
    TcpConnection(const std::shared_ptr<EventLoop>& eventLoop, const std::shared_ptr<Socket>& socket);
    ~TcpConnection();

    void HandleReadEvent(int fd);
    void HandleCloseEvent();
    void SetOnMessageCallback(const std::function<void(TcpConnection::ptr)>& callback);
    void SetDeleteConnectionCallBack(std::function<void(int)> deleteConnectionCallback);
    std::shared_ptr<http::HttpRequestParser> GetHttpRequestParser();

    TimeStamp GetTimeStamp();
    void UpdateTimeStamp(const TimeStamp& timestamp);

    void ResetReadBuffer(const std::string &readBuffer);
    void SetWriteBuffer(const std::string &writeBuffer);
    std::shared_ptr<Buffer> GetReadBuffer();
    std::shared_ptr<Buffer> GetWriteBuffer();
    void NonBlockRead();
    void Write(); // TODO 是否需要Non block


private:
    enum class State {
        INVALID = -1,
        CONNECTED = 0,
        DISCONNECTED = 1,
    };
    std::function<void(std::shared_ptr<Socket>)> m_handleReadEventCallback;
    std::function<void(int)> m_deleteConnectionCallback;
    std::function<void(TcpConnection::ptr)> m_onMessageCallback;
    
    std::shared_ptr<Channel> m_channel;
    std::shared_ptr<Socket> m_socket;
    std::shared_ptr<Buffer> m_readBuffer;
    std::shared_ptr<Buffer> m_writeBuffer;
    std::shared_ptr<http::HttpRequestParser> m_httpRequestParser;
    TimeStamp m_timeStamp;
    State m_state { State::INVALID };
};



#endif //CONNECTION_H
