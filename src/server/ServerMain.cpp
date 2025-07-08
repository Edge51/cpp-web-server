#include "Server.h"
#include "EventLoop.h"
#include "Logger.h"

int main(int argc, char* argv[])
{
    EventLoop::ptr eventLoop = std::make_shared<EventLoop>();
    Server::ptr server = std::make_shared<Server>(eventLoop);
    LOG("channels size[%d]\n", server->ConnectionCount());
    eventLoop->Start();
    return 0;
}
