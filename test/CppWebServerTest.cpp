#include <gtest/gtest.h>
#include <thread>

#include "CppWebServerTest.h"
#include "Server.h"
#include "Client.h"


TEST(CppWebServerTest, ServerRun)
{
    std::thread serverThread(ServerRun);
    serverThread.detach();
    sleep(3);
    EXPECT_EQ(ClientRequest(), "Hello from Server!\n");
    EXPECT_NE(ClientRequest(), "Hello from Server1!\n");
    EXPECT_EQ(ClientRequest(), "Hello from Server!\n");
}