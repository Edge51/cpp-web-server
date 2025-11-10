#include <fstream>
#include <unordered_map>

#include "logger.h"

Logger & Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

void Logger::SetLogAttr(const std::string &logFileName, LogLevel logLevel)
{
    m_logFileName = logFileName;
    m_logLevel = logLevel;
}

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
        {LogLevel::WARNING, "WARNING"},
        {LogLevel::FATAL, "FATAL"}
    };
    return logLevelMap.at(logLevel);
}

void Logger::Log(LogLevel level, const std::string &message, const std::string &filename, int line)
{
    if (level < m_logLevel) {
        return ;
    }
    std::fstream logFile(m_logFileName, std::ios::out | std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << LogLevelToString(level) << "] " << "[" << ExtractNameFromPath(filename) << ":" << line << "] ";
        logFile << message << std::endl;
    }
}
