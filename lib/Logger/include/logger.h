#ifndef LOGGER
#define LOGGER

#include <string>

int hello();

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};
class Logger {
public:
    Logger() = delete;
private:
    std::string m_logFileName;
    LogLevel m_logLevel;
};
#endif