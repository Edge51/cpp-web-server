//
// Created by Edge51 on 2025/9/2.
//

#include "EventLoopThread.h"

EventLoopThread::EventLoopThread()
    : m_eventLoop(nullptr)
{}

std::shared_ptr<EventLoop> EventLoopThread::GetStartLoop()
{
    // why we bind the threadFunc here?
    // because we wanna init a eventloop and bind a thread info with the eventloop
    // so we start a thread to run the loop here
    m_thread = std::thread([this] { LoopFuncForThread(); });
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_eventLoop == nullptr) {
            m_condition.wait(lock);
        }
        return m_eventLoop;
    }
}

void EventLoopThread::LoopFuncForThread()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_eventLoop = std::make_shared<EventLoop>();
        m_condition.notify_one();
    }
    m_eventLoop->Start();
}