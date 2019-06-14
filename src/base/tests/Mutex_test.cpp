#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>

#include "CountDownLatch.h"
#include "Mutex.hpp"
#include "Thread.h"
#include "Timestamp.h"

using namespace spero;
using namespace std;

MutexLock g_mutex;
vector<int> g_vec;
int maxCount = 1000*1000*1000;
int maxThreads = 64;

void threadFunc()
{
    for (int i = 0; i < maxCount; ++i) {
        MutexLockGuard lock(g_mutex);
        g_vec.push_back(i);
    }
}

int foo() __attribute__ ((noinline));

int g_count = 0;
int foo()
{
    MutexLockGuard lock(g_mutex);
    if (!g_mutex.isLockedByThisThread()) {
        printf("failed to lock mutex\n");
        return -1;
    }
    ++g_count;
    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc == 3) {
        int thread_num = atoi(argv[1]);
        int count_num = atoi(argv[2]);
        maxThreads = min(thread_num, maxThreads);
        maxCount = min (count_num, maxCount);
    }

    RET_CHECK(foo());
    if (g_count != 1) {
        printf("RET_CHECK calls twice, g_count:%d\n", g_count);
        abort();
    }
    
    try {
        g_vec.reserve(maxThreads * maxCount);
    }
    catch (...) {
        printf("reserve failed\n");
        exit(-1);
    }

    Timestamp start(Timestamp::now());
    for (int i = 0; i < maxCount; ++i) {
        g_vec.push_back(i);
    }
    printf("single thread without lock timeUsed: %f\n", timeDiff(Timestamp::now(), start));

    start = Timestamp::now();
    threadFunc();
    printf("single thread with lock timeUsed: %f\n", timeDiff(Timestamp::now(), start));

    for (int nthreads = 1; nthreads < maxThreads; ++nthreads) {
        g_vec.clear();
        vector<unique_ptr<Thread>> threads;
        start = Timestamp::now();
        
        for (int i = 0; i < nthreads; ++i) {
            threads.emplace_back(new Thread(&threadFunc));
            threads.back()->start();
        }
        
        for (int i = 0; i < nthreads; ++i) {
            threads[i]->join();
        }
       
        printf("%d thread(s) with lock timeUsed: %f\n", nthreads+1, timeDiff(Timestamp::now(), start));
    }
}

