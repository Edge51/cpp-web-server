#ifndef THREAD_QUEUE
#define THREAD_QUEUE

#include <queue>
#include <memory>
#include <thread>
#include <mutex>

template <typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue();
    ConcurrentQueue(const ConcurrentQueue& other) = delete;
    ConcurrentQueue(const ConcurrentQueue&& other) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue& other) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&& other) = delete;

    void Push(T item);
    void TryPop(T);
    void WaitAndPop(T);

private:
    struct Node {
        std::shared_ptr<T> data { nullptr };
        std::unique_ptr<Node> next { nullptr };
        Node() = delete;
        Node(std::shared_ptr<T> data_, std::unique_ptr<Node> next_) : data(data_), next(std::move(next_)) {}
    };

    std::mutex m_headMutex;
    std::unique_ptr<Node> m_head { nullptr };

    std::mutex m_tailMutex;
    Node* m_tail { nullptr };

    uint32_t m_size { 0 };

};

template <typename T>
ConcurrentQueue<T>::ConcurrentQueue()
{
    m_head
}


#endif