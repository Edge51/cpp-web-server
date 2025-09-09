//
// Created by Edge51 on 2025/9/1.
//

#ifndef CPPWEBSERVER_EVENTLOOPTHREADPOOL_H
#define CPPWEBSERVER_EVENTLOOPTHREADPOOL_H
#include <memory>
#include <vector>

#include <EventLoopThread.h>

class EventLoopThreadPool {
public:
    EventLoopThreadPool(int num_threads);
    ~EventLoopThreadPool() = default;
    std::shared_ptr<EventLoop> GetNextLoop();
    uint32_t Size() const;
private:
    std::vector<std::shared_ptr<EventLoopThread>> m_eventLoopThreads;
    std::vector<std::shared_ptr<EventLoop>> m_eventLoops;
    uint32_t m_nextLoopIndex { 0 };
};


#endif //CPPWEBSERVER_EVENTLOOPTHREADPOOL_H