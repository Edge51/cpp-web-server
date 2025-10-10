//
// Created by Edge51 on 2025/9/21.
//

#include "TimeStamp.h"

#include <time.h>
#include <sys/time.h>

TimeStamp::TimeStamp(int64_t microSeconds)
    : m_microSeconds(microSeconds)
{}

bool TimeStamp::operator==(const TimeStamp &other) const
{
    return m_microSeconds == other.m_microSeconds;
}

bool TimeStamp::operator<(const TimeStamp &other) const
{
    return m_microSeconds < other.m_microSeconds;
}

std::string TimeStamp::ToFormatedString() const
{
    tm tm_time;
    localtime_r(&m_microSeconds, &tm_time);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%D %T", &tm_time);
    return buffer;
}

int64_t TimeStamp::MicroSeconds() const
{
    return m_microSeconds;
}

TimeStamp TimeStamp::Now()
{
    timeval now {};
    gettimeofday(&now, nullptr);
    return TimeStamp(now.tv_usec);
}

TimeStamp TimeStamp::AddMicroSeconds(const TimeStamp &old, int64_t microSeconds)
{
    int64_t new_microSeconds = old.m_microSeconds + microSeconds;
    if (new_microSeconds >= 0) {
        return TimeStamp(new_microSeconds);
    }
    return TimeStamp(old.m_microSeconds - microSeconds);
}
