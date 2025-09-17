//
// Created by Edge51 on 2025/9/1.
//

#include "EventLoopThreadPool.h"
#include "Logger.h"

EventLoopThreadPool::EventLoopThreadPool(int num_threads)
{
    LOG("EventLoopThreadPool::EventLoopThreadPool");
    for (int i = 0; i < num_threads; ++i) {
        LOG("EventLoopThreadPool[%d]", i);
        m_eventLoopThreads.push_back(std::make_shared<EventLoopThread>());
        LOG("EventLoopThreadPool[%d]", i);
        m_eventLoops.emplace_back(m_eventLoopThreads[i]->GetStartLoop());
        LOG("EventLoopThreadPool[%d]", i);
    }
}

std::shared_ptr<EventLoop> EventLoopThreadPool::GetNextLoop()
{
    if (m_eventLoops.empty()) {
        return nullptr;
    }
    auto ret = m_eventLoops[m_nextLoopIndex++];
    if (m_nextLoopIndex == m_eventLoops.size()) {
        m_nextLoopIndex = 0;
    }
    return ret;
}

uint32_t EventLoopThreadPool::Size() const
{
    return static_cast<uint32_t>(m_eventLoops.size());
}