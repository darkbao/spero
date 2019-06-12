#include <stdio.h>

#include <sys/prctl.h>

#include "Thread.h"
#include "CurrentThread.h"
#include "Exception.h"

namespace spero
{

namespace detail
{

void childAfterFork()
{
    CurrentThread::t_cachedTid = 0;
    CurrentThread::t_threadName = "main";
    CurrentThread::tid();
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        CurrentThread::t_threadName = "main";
        CurrentThread::tid();
        pthread_atfork(NULL, NULL, &childAfterFork);
    }
};

ThreadNameInitializer init;

struct ThreadData
{
    typedef Thread::ThreadFunc ThreadFunc;

    ThreadFunc      func_;
    std::string     name_;
    pid_t*          tid_;
    CountDownLatch* latch_;

    ThreadData(ThreadFunc func, const std::string& name, pid_t* tid, CountDownLatch* latch)
        : func_(std::move(func)), name_(name), tid_(tid), latch_(latch)
    { }

    void runInThread()
    {
        *tid_ = CurrentThread::tid();
        tid_  = NULL;
        latch_->countDown();
        latch_ = NULL;

        CurrentThread::t_threadName = name_.empty() ? "speroThread" : name_.c_str();
        ::prctl(PR_SET_NAME, CurrentThread::t_threadName);
        try {
            func_();
            CurrentThread::t_threadName = "finished";
        }
        catch (const Exception& ex) {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            abort();
        }
        catch (const std::exception& ex) {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch (...) {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
            throw; // rethrow
        }
    }
};

void* startThread(void* obj)
{
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

} // namespace detail



AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string& str)
    : started_(false),
      joined_(false),
      tid_(0),
      pthreadID_(0),
      func_(std::move(func)),
      name_(str),
      latch_(1)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (started_ && !joined_) {
        pthread_detach(pthreadID_);
    }
}

void Thread::setDefaultName()
{
    int num = numCreated_.incrementAndGet();
    if (name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

void Thread::start()
{
    assert(started_ == false);
    started_ = true;
    // FIXME: move(func_)
    detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_, &latch_);
    int ret = pthread_create(&pthreadID_, NULL, &detail::startThread, data);
    if (ret != 0) {
        started_ = false;
        delete data; // or no delete?
        printf("failed in pthread_create, ret:%d\n", ret);
    } else {
        // wait for countdown
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join()
{
  assert(started_ == true);
  assert(joined_  == false);
  joined_ = true;
  return pthread_join(pthreadID_, NULL);
}


} // namespace spero