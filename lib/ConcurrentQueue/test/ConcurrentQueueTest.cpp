#include <gtest/gtest.h>
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
    
}