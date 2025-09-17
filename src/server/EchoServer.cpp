#include <unistd.h>

#include "tcp/TcpServer.h"
#include "tcp/EventLoop.h"
#include "tcp/Buffer.h"
#include "tcp/Logger.h"

int main(int argc, char* argv[])
{
    auto eventLoop = std::make_shared<EventLoop>();
    auto server = std::make_shared<TcpServer>(eventLoop);
    server->SetOnMessage([](std::shared_ptr<TcpConnection> conn) {
        conn->NonBlockRead();
        auto readBuffer = conn->GetReadBuffer();
        LOG("Message from client:%s", readBuffer->c_str());
        conn->SetWriteBuffer(readBuffer->c_str());
        conn->Write();
    });

    eventLoop->Start();
    return 0;
}
