//
// Created by Edge51 on 2025/9/18.
//

#ifndef CPPWEBSERVER_HTTPSERVER_H
#define CPPWEBSERVER_HTTPSERVER_H
#include <functional>
#include <memory>

#include "TcpServer.h"

namespace http {
    class HttpRequest;
    class HttpResponse;
}

class EventLoop;
class TcpConnection;
namespace http {

constexpr int64_t AUTO_CLOSE_TIMEOUT = 5000;

class HttpServer {
public:
    explicit HttpServer(std::shared_ptr<EventLoop> loop, bool autoClose = false);
    void SetResponseCallback(std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> func);
private:
    void OnConnect(const TcpConnection::ptr& tcpConnection);
    void OnMessage(std::shared_ptr<TcpConnection> tcpConnection);
    void OnRequest(TcpConnection::ptr tcpConnection, HttpRequest request);
    void AutoCloseTimeoutConnection(std::weak_ptr<TcpConnection> tcpConnection);

private:
    std::shared_ptr<TcpServer> m_tcpServer;
    std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> m_response_callback;
    std::shared_ptr<EventLoop> m_mainReactor;
    bool m_autoCloseFlag;
};

} // namespace http

#endif //CPPWEBSERVER_HTTPSERVER_H