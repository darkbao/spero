#ifndef SPERO_BASE_COUNTDOWNLATCH_H
#define SPERO_BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.hpp"

namespace spero
{

class CountDownLatch : public NonCopyable
{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int  getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};

}  // namespace spero
#endif  // SPERO_BASE_COUNTDOWNLATCH_H
