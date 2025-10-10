//
// Created by Edge51 on 2025/9/21.
//

#include "Timer.h"

Timer::Timer(TimeStamp timeStamp, std::function<void()> callback, int64_t microSeconds)
    : m_expirationTime(timeStamp),
    m_callback(callback),
    m_intervalMicroSeconds(microSeconds),
    m_isRepeatEnabled(microSeconds > 0)
{
}

Timer::Timer()
    :m_expirationTime(TimeStamp::Now())
{
}


TimeStamp Timer::ExpirationTime() const
{
    return m_expirationTime;
}

void Timer::Run()
{
    m_callback();
}

bool Timer::IsRepeatEnabled() const
{
    return m_isRepeatEnabled;
}

void Timer::Restart(TimeStamp now)
{
    m_expirationTime = TimeStamp::AddMicroSeconds(now, m_intervalMicroSeconds);
}
