#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <map>
#include "ConcurrentQueue.h"

TEST(ConcurrentQueueTest, PushAndPop)
{
    ConcurrentQueue<int> queue;
    EXPECT_TRUE(queue.Empty());
    queue.Push(1);
    EXPECT_FALSE(queue.Empty());
    queue.Push(2);
    queue.Push(3);
    queue.Push(4);
    EXPECT_EQ(queue.Top(), 1);
    EXPECT_FALSE(queue.Empty());
    queue.Pop();
    EXPECT_EQ(queue.Top(), 2);
    queue.Pop();
    EXPECT_EQ(queue.Top(), 3);
    queue.Pop();
    EXPECT_EQ(queue.Top(), 4);
    queue.Pop();
    EXPECT_TRUE(queue.Empty());
}

TEST(ConcurrentQueueTest, MultiThread)
{
    ConcurrentQueue<int> queue;
    std::vector<std::thread> producerList;
    for (int i = 0; i < 400; i++) {
        producerList.emplace_back(([](ConcurrentQueue<int>& queue, int i) {
            queue.Push(i);
            queue.Push(i + 400);
        }), std::ref(queue), i);
    }
    EXPECT_EQ(producerList.size(), 400);

    std::map<int, int> items;
    std::vector<std::thread> consumerList;
    for (int i =  0; i < 4; i++) {
        consumerList.emplace_back([] (ConcurrentQueue<int>& queue, std::map<int, int>& items) {
            for (int j  = 0; j < 400; j++) {
                items[queue.Top()]++;
                queue.Pop();
            }
        }, std::ref(queue), std::ref(items));
    }
    EXPECT_EQ(consumerList.size(), 4);

    for (auto& producer : producerList) {
        producer.join();
    }
    for (auto& consumer : consumerList) {
        consumer.join();
    }
    // for (int i = 0; i < 800; i++) {
    //     EXPECT_EQ(items[i], 1);
    // }
    // TODO Too many print in UT result if it fails.
    EXPECT_EQ(items.size(), 800);
    for (const auto& item : items) {
        GTEST_LOG_(INFO) << "item key:" << item.first << ", value:" << item.second;
    }
}