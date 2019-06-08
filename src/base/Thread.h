#ifndef SPERO_BASE_THREAD_H
#define SPERO_BASE_THREAD_H

#include <functional>
//#include <memory>
#include <pthread.h>

#include "Atomic.hpp"
#include "CountDownLatch.h"

namespace spero
{

class Thread : public NonCopyable
{
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(ThreadFunc, const std::string& name = std::string());
    // FIXME: make it movable in C++11
    
    ~Thread();

    void start();
    
    int join(); // return pthread_join()

    bool started() const { return started_; }
    
    // pthread_t pthreadId() const { return pthreadID_; }
    
    pid_t tid() const { return tid_; }
    
    const std::string& name() const { return name_; }

    static int numCreated() { return numCreated_.get(); }

protected:
    void setDefaultName();

private:
    bool           started_;
    bool           joined_;
    pid_t          tid_;
    pthread_t      pthreadID_;
    ThreadFunc     func_;
    std::string    name_;
    CountDownLatch latch_;

    static AtomicInt32 numCreated_;
    
};

} // namespace spero

#endif // SPERO_BASE_THREAD_H