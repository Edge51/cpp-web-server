#ifndef LOGGER
#define LOGGER
#include <atomic>
#include <thread>


#ifdef LOGGER_EXPORTS
#define LOGGER_API __attribute__((visibility("default")))
#else
#define LOGGER_API
#endif

#include <cstdint>
#include <string>
#include <memory>
#include <fstream>

#include "DisableCopy.h"

namespace ELogger {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class DoubleBuffer;
class LogFormatter;

class LOGGER_API Logger : public DisableCopy {
public:
    static Logger& GetInstance();
    void SetLogAttr(const std::string& logFileName, LogLevel logLevel);
    void Log(LogLevel level, const std::string &message, const std::string &filename, int line);
    void Stop();

protected:
    Logger();
    ~Logger() = default;
private:
    void FlushThreadFunc();
    void OpenLogFile();
    void CloseLogFile();
    void WriteToLogFile(const char* msg, uint32_t len);

private:
    std::atomic<bool> isRunning_;
    std::string logFileName_;
    std::fstream logFileStream_;
    LogLevel logLevel_;
    std::unique_ptr<LogFormatter> formatter_;
    std::unique_ptr<DoubleBuffer> buffer_;
    std::thread flushThread_;
};

#define LOG_INIT(logFileName, logLevel) Logger::GetInstance().SetLogAttr(logFileName, logLevel)

#define LOG_DEINIT() Logger::GetInstance().Stop()

#define LOG_INFO(message) Logger::GetInstance().Log(LogLevel::INFO, message, __FILE__, __LINE__)

#define LOG_DEBUG(message) Logger::GetInstance().Log(LogLevel::DEBUG, message, __FILE__, __LINE__)

#define LOG_ERROR(message) Logger::GetInstance().Log(LogLevel::ERROR, message, __FILE__, __LINE__)

#define LOG_WARN(message) Logger::GetInstance().Log(LogLevel::WARNING, message, __FILE__, __LINE__)

#define LOG_FATAL(message) Logger::GetInstance().Log(LogLevel::FATAL, message, __FILE__, __LINE__)


} // namespace Logger

#endif
