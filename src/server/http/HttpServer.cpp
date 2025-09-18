//
// Created by Edge51 on 2025/9/18.
//

#include "HttpServer.h"

#include "Buffer.h"
#include "HttpRequestParser.h"
#include "HttpResponse.h"
#include "Logger.h"
#include "TcpConnection.h"

namespace http {
    HttpServer::HttpServer(std::shared_ptr<EventLoop> loop) : m_tcpServer(std::make_shared<TcpServer>(loop))
    {
        m_tcpServer->SetOnMessage([this](TcpConnection::ptr tcpConnection) {
            OnMessage(tcpConnection);
        });
    }

    void HttpServer::SetResponseCallback(
        std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> func)
    {
        m_response_callback = func;
    }

    void HttpServer::OnConnect(TcpConnection::ptr tcpConnection)
    {
        LOG("New connection!");
    }

    void HttpServer::OnMessage(TcpConnection::ptr tcpConnection)
    {
        if (!tcpConnection) {
            LOG("tcpConnection nullptr!!!");
        }
        HttpRequestParser::ptr httpParser = tcpConnection->GetHttpRequestParser();
        if (httpParser == nullptr) {
            tcpConnection->SetWriteBuffer("close");
            tcpConnection->Write();
        }
        tcpConnection->NonBlockRead();
        Buffer::ptr readBuffer = tcpConnection->GetReadBuffer();
        httpParser->AppendData(readBuffer->c_str());
        httpParser->Parse();
        if (httpParser->IsComplete() && httpParser->HasError()) {
            tcpConnection->SetWriteBuffer("HTTP/1.1 400 Bad Request\r\n\r\n");
            tcpConnection->Write();
        }
        if (httpParser->IsComplete() && !httpParser->HasError()) {
            OnRequest(tcpConnection, httpParser->GetHttpRequest());
        }
    }

    void HttpServer::OnRequest(TcpConnection::ptr tcpConnection, HttpRequest request)
    {
        std::string connectionState = request.GetHeader("Connection");
        bool isClose = connectionState == "close" ||
            (request.GetVersion() == HttpVersion::HTTP_1_0 && connectionState != "keep-alive");
        HttpResponse::ptr response = std::make_shared<HttpResponse>();
        response->SetCloseFlag(isClose);
        m_response_callback(std::make_shared<HttpRequest>(request), response);
        tcpConnection->SetWriteBuffer(response->PlainText());
        tcpConnection->Write();
    }
} // namespace http