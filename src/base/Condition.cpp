#include <errno.h>
#include "Condition.h"

using namespace spero;

Condition::Condition(MutexLock& mlk) : mLock_(mlk)
{
    RET_CHECK(pthread_cond_init(&cond_, NULL));
}

Condition::~Condition()
{
    RET_CHECK(pthread_cond_destroy(&cond_));
}

bool Condition::waitForSeconds(time_t second, long nanosecond)
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    tp.tv_sec  += second;
    tp.tv_nsec += nanosecond;

    MutexLock::OwnerGuard og(mLock_);
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mLock_.getPthreadMutex(), &tp);
}

void Condition::wait()
{
    MutexLock::OwnerGuard og(mLock_);
    RET_CHECK(pthread_cond_wait(&cond_, mLock_.getPthreadMutex()));
}

void Condition::notify()
{
    RET_CHECK(pthread_cond_signal(&cond_));
}

void Condition::notifyAll()
{
    RET_CHECK(pthread_cond_broadcast(&cond_));
}


