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

TEST(LoggerTest, LogTest)
{
    LOG_INIT("logTest.log", LogLevel::INFO);
    LOG_DEBUG("DEBUG");
    LOG_INFO("INFO");
    LOG_WARN("WARN");
    LOG_ERROR("ERROR");
    LOG_FATAL("FATAL");
    LOG_DEINIT();
}