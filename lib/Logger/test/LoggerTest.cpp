//
// Created by Edge51 on 2025/11/9.
//

#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "../src/DoubleBuffer.h"
#include "logger.h"
#include "../src/FixedBuffer.h"

using namespace ELogger;

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

TEST(LoggerTest, FixedBuffer)
{
    FixedBuffer fb;
    fb.Append("hello");
    fb.Append(" world");
    EXPECT_STREQ(fb.Data(), "hello world");
    EXPECT_STRNE(fb.Data(), "helloworld");
    EXPECT_EQ(fb.AvailSize(), BUFFER_SIZE - 11);
    EXPECT_EQ(fb.UsedSize(), 11);
    fb.Reset();
    EXPECT_EQ(fb.UsedSize(), 0);
    EXPECT_EQ(fb.AvailSize(), BUFFER_SIZE);
}

TEST(LoggerTest, DoubleBuffer)
{
    DoubleBuffer db;
    std::string output;
    db.SetFlushCallback([&output](const char* data, uint32_t length) {
        output.append(data, length);
    });
    db.Append("hello");
    db.Append(" world");
    db.Flush();
    db.Stop();
    EXPECT_STREQ(output.data(), "hello world");
}