
#include <gtest/gtest.h>

#include "CppWebServerTest.h"
#include "logger.h"

TEST(CppWebServerTest, Initialize)
{
    EXPECT_EQ(hello(), 1);
    EXPECT_NE(hello(), 0);
}