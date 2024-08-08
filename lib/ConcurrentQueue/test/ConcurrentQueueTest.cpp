#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <map>
#include <sstream>
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

std::mutex g_popMutex;
TEST(ConcurrentQueueTest, MultiThread)
{
    ConcurrentQueue<int> queue;
    std::vector<std::thread> producerList;
    const uint32_t producerNum = 400;
    const uint32_t consumerNum = 80;

    std::vector<int> pushingLog;
    for (int i = 0; i < producerNum; i++) {
        producerList.emplace_back(([&](ConcurrentQueue<int>& queue, int i) {
            queue.Push(i);
            GTEST_LOG_(INFO) << "pushing:" << i;
            pushingLog.push_back(i);
            queue.Push(i + producerNum);
            GTEST_LOG_(INFO) << "pushing:" << i + producerNum;
            pushingLog.push_back(i + producerNum);
        }), std::ref(queue), i);
    }
    EXPECT_EQ(producerList.size(), producerNum);

    std::map<int, int> items;
    std::vector<std::thread> consumerList;
    std::vector<int> popingLog;
    for (int i =  0; i < consumerNum; i++) {
        consumerList.emplace_back([&] (ConcurrentQueue<int>& queue, std::map<int, int>& items) {
            for (int j  = 0; j < producerNum * 2 / consumerNum; j++) {
                std::lock_guard guard(g_popMutex);
                int top = queue.Top();
                items[top]++;
                GTEST_LOG_(INFO) << "poping:" << queue.Top();
                popingLog.push_back(top);
                queue.Pop();
            }
        }, std::ref(queue), std::ref(items));
    }
    EXPECT_EQ(consumerList.size(), consumerNum);

    for (auto& producer : producerList) {
        producer.join();
    }
    for (auto& consumer : consumerList) {
        consumer.join();
    }
    // for (int i = 0; i < 800; i++) {
    //     EXPECT_EQ(items[i], 1);
    // }
    std::stringstream pushingSstream;
    for (const auto& log : pushingLog) {
        pushingSstream << log << " ";
    }
    char product[256];
    pushingSstream.getline(product, 256);
    GTEST_LOG_(INFO) << "pushingLog:" << product;

    std::stringstream popingSstream;
    for (const auto& log : popingLog) {
        popingSstream << log << " ";
    }
    char consume[256];
    popingSstream.getline(consume, 256);
    GTEST_LOG_(INFO) << "popingLog:" << consume;
    // TODO Too many print in UT result if it fails.
    EXPECT_EQ(items.size(), producerNum * 2);
    // for (const auto& item : items) {
    //     GTEST_LOG_(INFO) << "item key:" << item.first << ", value:" << item.second;
    // }
}