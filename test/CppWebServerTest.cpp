#include <gtest/gtest.h>
#include <thread>

#include "CppWebServerTest.h"
#include "TcpServer.h"
#include "Client.h"
#include <cstdio>

// TEST(CppWebServerTest, ServerRun)
// {
//     printf("ServerRun\n");
//     sleep(3);
//     std::thread serverThread(ServerRun);
//     serverThread.detach();
//     sleep(3);
//     EXPECT_EQ(ClientRequest(), "Hello from Server!\n");
//     EXPECT_NE(ClientRequest(), "Hello from Server1!\n");
//     EXPECT_EQ(ClientRequest(), "Hello from Server!\n");
// }