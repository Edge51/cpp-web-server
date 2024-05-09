#ifndef THREADPOOL_THREADPOOLTEST_H
#define THREADPOOL_THREADPOOLTEST_H

#include <gtest/gtest.h>

class ThreadPoolTest : public ::testing::Test {
public:
	void SetUp() override;
	void TearDown() override;
};


#endif //THREADPOOL_THREADPOOLTEST_H