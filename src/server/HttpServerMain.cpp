//
// Created by Edge51 on 2025/9/18.
//

#include "http/HttpServer.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "http/HttpDefine.h"
#include "EventLoop.h"

using namespace http;

void ResponseCallBack(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response)
{
    if (request->GetMethod() != HttpMethod::GET){
        response->SetStatusCode(500);
        response->SetStatusDescription("Bad Request");
        response->SetCloseFlag(true);
    }

    {
        const std::string html = " <font color=\"green\">This is Edge51's website!!!</font> ";
        std::string url = request->GetUrl();
        if(url == "/"){
            response->SetStatusCode(200);
            response->SetBody(html);
            response->SetContentType("text/html");
        }else if(url == "/hello"){
            response->SetStatusCode(200);
            response->SetBody("hello world\n");
            response->SetContentType("text/plain");
        }else if(url == "/favicon.ico"){
            response->SetStatusCode(200);
        }else{
            response->SetStatusCode(404);
            response->SetStatusDescription("Not Found");
            response->SetBody("Sorry Not Found\n");
            response->SetCloseFlag(true);
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    auto eventLoop = std::make_shared<EventLoop>();
    auto server = std::make_shared<HttpServer>(eventLoop);
    server->SetResponseCallback(ResponseCallBack);
    eventLoop->Start();
}
