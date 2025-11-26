#include <fstream>
#include <iostream>

#include "logger.h"
#include "LogFormatter.h"
#include "DoubleBuffer.h"

namespace ELogger {

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

void Logger::SetLogAttr(const std::string &logFileName, LogLevel logLevel)
{
    logFileName_ = logFileName;
    logLevel_ = logLevel;
    formatter_ = std::make_unique<SimpleLogFormatter>();
    buffer_->SetFlushCallback([this](const char* data, uint32_t len) {
        WriteToLogFile(data, len);
    });
}

void Logger::Log(LogLevel level, const std::string &message, const std::string &filename, int line)
{
    if (level < logLevel_) {
        return ;
    }
    buffer_->Append(formatter_->Format(level, message, filename, line));
}

void Logger::Stop()
{
    if (!isRunning_) {
        return;
    }
    isRunning_ = false;
    if (flushThread_.joinable()) {
        flushThread_.join();
    }

    buffer_->Stop();
    CloseLogFile();
}

Logger::Logger()
    :buffer_(std::make_unique<DoubleBuffer>()), isRunning_(true)
{
    flushThread_ = std::thread(&Logger::FlushThreadFunc, this);
}

void Logger::FlushThreadFunc()
{
    while (isRunning_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        buffer_->Flush();
    }
}

void Logger::OpenLogFile()
{
    if (!logFileStream_.is_open()) {
        logFileStream_.open(logFileName_, std::ios::out | std::ios::app);
    }
}

void Logger::CloseLogFile()
{
    if (logFileStream_.is_open()) {
        logFileStream_.close();
    }
}

void Logger::WriteToLogFile(const char *msg, uint32_t len)
{
    if (!logFileStream_.is_open()) {
        OpenLogFile();
    }
    logFileStream_.write(msg, len);
    logFileStream_.flush();
}
} // namespace ELogger
