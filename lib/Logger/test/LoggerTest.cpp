//
// Created by Edge51 on 2025/11/9.
//

#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "logger.h"

TEST(LoggerTest, simpleLog)
{
    LOG_INIT("test.log", LogLevel::DEBUG);
    LOG_DEBUG("hello");
    std::fstream logFile("test.log", std::ios::in | std::ios::app);
    ASSERT_TRUE(logFile.is_open());
    std::stringstream ss;
    ss << logFile.rdbuf();
    std::string content = ss.str();
    GTEST_LOG_(INFO) << "content:" << content;
    EXPECT_NE(content.find("hello"), std::string::npos);
}