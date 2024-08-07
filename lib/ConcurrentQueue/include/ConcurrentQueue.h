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
    void Pop();
    T Top();
    bool Empty() const;
    uint32_t Size() const;

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
:m_head(std::make_unique<Node>(nullptr, nullptr)), m_tail(m_head.get()), m_size(0) {}

template <typename T>
void ConcurrentQueue<T>::Push(T item)
{
    std::shared_ptr<T> data = std::make_shared<T>(std::move(item));
    std::unique_ptr<Node> newNode = std::make_unique<Node>(nullptr, nullptr);
    m_tail->data = data;
    m_tail->next = std::move(newNode);
    m_tail = m_tail->next.get();
    m_size++;
}

#include <iostream>
template <typename T>
T ConcurrentQueue<T>::Top()
{
    if (m_size <= 0) {
        std::cout << "m_size:%u <= -1" << std::endl;
        return T();
    }
    return *(m_head->data);
}

template <typename T>
void ConcurrentQueue<T>::Pop()
{
    // TODO Should I throw an exception here?
    if (m_size <= 0) {
        return ;
    }
    m_head = std::move(m_head->next);
    m_size--;
}

template <typename T>
bool ConcurrentQueue<T>::Empty() const
{
    return m_size <= 0;
}

template <typename T>
uint32_t ConcurrentQueue<T>::Size() const
{
    return m_size;
}
#endif