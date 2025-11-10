#ifndef LOGGER
#define LOGGER

#ifdef LOGGER_EXPORTS
#define LOGGER_API __attribute__((visibility("default")))
#else
#define LOGGER_API
#endif

#include <cstdint>
#include <string>
#include <memory>

#include "DisableCopy.h"

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class LOGGER_API Logger : public DisableCopy {
public:
    static Logger& GetInstance();
    void SetLogAttr(const std::string& logFileName, LogLevel logLevel);
    void Log(LogLevel level, const std::string &message, const std::string &filename, int line);

protected:
    Logger() = default;
    ~Logger() = default;

private:
    std::string m_logFileName;
    LogLevel m_logLevel;
};

#define LOG_INIT(logFileName, logLevel) Logger::GetInstance().SetLogAttr(logFileName, logLevel)
#define LOG_DEINIT() Logger::GetInstance().SetLogAttr("", LogLevel::INFO)
#define LOG_INFO(message) Logger::GetInstance().Log(LogLevel::INFO, message, __FILE__, __LINE__)
#define LOG_DEBUG(message) Logger::GetInstance().Log(LogLevel::DEBUG, message, __FILE__, __LINE__)
#define LOG_ERROR(message) Logger::GetInstance().Log(LogLevel::ERROR, message, __FILE__, __LINE__)
#define LOG_WARN(message) Logger::GetInstance().Log(LogLevel::WARNING, message, __FILE__, __LINE__)
#define LOG_FATAL(message) Logger::GetInstance().Log(LogLevel::FATAL, message, __FILE__, __LINE__)


#endif
