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

class HttpServer {
public:
    explicit HttpServer(std::shared_ptr<EventLoop> loop);
    void SetResponseCallback(std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> func);
private:
    void OnConnect(TcpConnection::ptr tcpConnection);
    void OnMessage(std::shared_ptr<TcpConnection> tcpConnection);
    void OnRequest(TcpConnection::ptr tcpConnection, HttpRequest request);

private:
    std::shared_ptr<TcpServer> m_tcpServer;
    std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> m_response_callback;
};

} // namespace http

#endif //CPPWEBSERVER_HTTPSERVER_H