// thread_pool.cpp : Defines the entry point for the application.
//
#include "gtest/gtest.h"

#include "thread_pool.h"
#include "worker.hpp"
using namespace std;


TEST(ThreadPool, sync_call)
{
    Worker worker;
    worker.Initialze();

    int ans = worker.sync_call([] {
        int ans = 0;
        for (int i = 0; i < 10; i++) ans += i;
        return ans;
    });
    EXPECT_EQ(ans,45);

    ans = worker.sync_call([&worker] {
        return worker.sync_call([] {
            int ans = 0;
            for (int i = 0; i < 10; i++) ans += i;
            return ans;
        });
    });

    EXPECT_EQ(ans,2);
    worker.Wait();
}

