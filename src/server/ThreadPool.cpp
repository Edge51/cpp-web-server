//
// Created by liuyu on 7/6/2025.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : m_running(true){
    for (int i = 0; i < size; i++) {
        m_workers.emplace_back([this]() {
            while (m_running) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_cv.wait(lock, [this]() {
                        return !m_running || !m_tasks.empty();
                    });
                    if (!m_running && m_tasks.empty()) {
                        return ;
                    }
                    task = m_tasks.front();
                    m_tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    m_running = false;
}
