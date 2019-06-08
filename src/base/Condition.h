#ifndef SPERO_BASE_CONDITION_H
#define SPERO_BASE_CONDITION_H

#include <pthread.h>
#include "Mutex.hpp"

namespace spero
{

class Condition : public NonCopyable
{
public:
    explicit Condition(MutexLock& mutex);
    ~Condition();

    // returns true if time out, false otherwise.
    bool waitForSeconds(time_t second, long nanosecond = 0);
    void wait();
    void notify();
    void notifyAll();

private:
    pthread_cond_t cond_;
    MutexLock& mLock_;
};

}

#endif // SPERO_BASE_CONDITION_H