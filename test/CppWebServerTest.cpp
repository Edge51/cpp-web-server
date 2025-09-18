#include <gtest/gtest.h>
#include <thread>

#include "CppWebServerTest.h"
#include "TcpServer.h"
#include "Client.h"
#include <cstdio>

#include "Logger.h"
#include "EventLoop.h"
#include "Buffer.h"
#include "TcpConnection.h"

void TcpServerRun()
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
}

TEST(CppWebServerTest, ServerRun)
{
    printf("ServerRun\n");
    sleep(3);
    std::thread serverThread(TcpServerRun);
    serverThread.detach();
    sleep(3);
    EXPECT_EQ(ClientRequest(), "hello from client");
    EXPECT_NE(ClientRequest(), "Hello from Server1!");
    EXPECT_EQ(ClientRequest(), "hello from client");
}