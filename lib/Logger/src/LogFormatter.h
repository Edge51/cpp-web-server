//
// Created by Edge51 on 2025/10/20.
//

#ifndef CPPWEBSERVER_LOGFORMATTER_H
#define CPPWEBSERVER_LOGFORMATTER_H
#include <string>

#include "logger.h"

namespace ELogger {

class LogFormatter {
public:
    LogFormatter() = default;
    virtual std::string Format(LogLevel level, const std::string& message,
        const std::string& fileName, int lineNumber) = 0;
    virtual ~LogFormatter() = default;
};
class SimpleLogFormatter : public LogFormatter {
public:
    std::string Format(LogLevel level, const std::string& message, const std::string& fileName, int lineNumber);
};

} // namespace Logger

#endif //CPPWEBSERVER_LOGFORMATTER_H