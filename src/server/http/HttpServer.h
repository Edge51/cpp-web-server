//
// Created by Edge51 on 2025/9/18.
//

#ifndef CPPWEBSERVER_HTTPSERVER_H
#define CPPWEBSERVER_HTTPSERVER_H

namespace http {


class HttpServer {
public:
    HttpServer();
    void Start();
private:
    void OnConnect();
    void OnMessage();
    void OnRequest();

private:
};

} // namespace http

#endif //CPPWEBSERVER_HTTPSERVER_H