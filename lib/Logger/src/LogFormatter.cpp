//
// Created by Edge51 on 2025/10/20.
//

#include "LogFormatter.h"

#include <iomanip>
#include <sstream>
#include <unordered_map>

namespace ELogger {
    std::string ExtractNameFromPath(const std::string &path)
    {
        auto pos = path.find_last_of("/\\");
        if (pos != std::string::npos) {
            return path.substr(pos + 1);
        }
        return path;
    }

    std::string LogLevelToString(LogLevel logLevel)
    {
        std::unordered_map<LogLevel, std::string> logLevelMap = {
            {LogLevel::INFO, "INFO"},
            {LogLevel::DEBUG, "DEBUG"},
            {LogLevel::ERROR, "ERROR"},
            {LogLevel::WARNING, "WARN"},
            {LogLevel::FATAL, "FATAL"}
        };
        return logLevelMap.at(logLevel);
    }

    std::string GetTimeStamp()
    {
        std::ostringstream ss;
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        ss << std::put_time(std::localtime(&t), "%F %T");
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    std::string SimpleLogFormatter::Format(LogLevel level, const std::string &message, const std::string &fileName,
                                           int lineNumber)
    {
        std::stringstream ss;
        ss << "[" << GetTimeStamp() << "] " << "[T:" << std::this_thread::get_id() << "]";
        ss << "[" << LogLevelToString(level) << "] " << "[" << ExtractNameFromPath(fileName) << ":" << lineNumber <<
                "] ";
        ss << message << std::endl;
        return ss.str();
    }
} // namespace ELogger
