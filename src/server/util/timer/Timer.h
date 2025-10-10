//
// Created by Edge51 on 2025/9/21.
//

#ifndef CPPWEBSERVER_TIMER_H
#define CPPWEBSERVER_TIMER_H
#include <functional>

#include "TimeStamp.h"


class Timer {
public:
    Timer(TimeStamp timeStamp, std::function<void()> callback, int64_t intervalMicroSeconds);
    Timer();
    TimeStamp ExpirationTime() const;

    void Run();
    bool IsRepeatEnabled() const;
    void Restart(TimeStamp now);
private:
    TimeStamp m_expirationTime;
    std::function<void()> m_callback;
    int64_t m_intervalMicroSeconds;
    bool m_isRepeatEnabled;
};


#endif //CPPWEBSERVER_TIMER_H