//
// Created by liuyu on 7/6/2025.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>
#include <condition_variable>
#include <future>

constexpr int32_t THREAD_POLL_DEFAULT_SIZE = 20;

class ThreadPool {
public:
    ThreadPool(int size);
    ~ThreadPool();
    template <typename F, typename... Args>
    auto AddTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
private:
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::atomic<bool> m_running;
    std::condition_variable m_cv;
    std::mutex m_mutex;
};

template <typename F, typename... Args>
auto ThreadPool::AddTask(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>> {
    using ResultType = std::result_of_t<F(Args...)>;
    auto task = std::make_shared<std::packaged_task<ResultType()>> (
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<ResultType> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_running) {
            throw std::runtime_error("ThreadPool::AddTask: Failed to add task, thread pool not running");
        }
        m_tasks.emplace([task]() { (*task)(); });
    }
    m_cv.notify_one();
    return res;
}

#endif //THREADPOOL_H
