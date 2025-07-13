#include <unistd.h>

#include "Server.h"
#include "EventLoop.h"
#include "Buffer.h"
#include "Logger.h"

int main(int argc, char* argv[])
{
    auto eventLoop = std::make_shared<EventLoop>();
    auto server = std::make_shared<Server>(eventLoop);
    server->SetOnConnect([](std::shared_ptr<Connection> conn) {
        conn->NonBlockRead();
        auto readBuffer = conn->GetReadBuffer();
        LOG("Message from client:%s", readBuffer->c_str());
        conn->SetWriteBuffer(readBuffer->c_str());
        conn->Write();
    });

    eventLoop->Start();
    return 0;
}
