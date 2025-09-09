//
// Created by Edge51 on 2025/9/2.
//

#ifndef CPPWEBSERVER_EVENTLOOPTHREAD_H
#define CPPWEBSERVER_EVENTLOOPTHREAD_H

#include <condition_variable>
#include <mutex>
#include <thread>

#include "EventLoop.h"

class EventLoopThread {
public:
    EventLoopThread();
    ~EventLoopThread() = default;

    /**
     * @brief Start EventLoop->Loop and return EventLoop,
     *        to avoid race condition, we did not split
     *        this two action into different interfaces
     * @return std::shared_ptr<EventLoop>
     */
    std::shared_ptr<EventLoop> GetStartLoop();
private:
    void LoopFuncForThread();

private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::shared_ptr<EventLoop> m_eventLoop;
    std::thread m_thread;
};


#endif //CPPWEBSERVER_EVENTLOOPTHREAD_H