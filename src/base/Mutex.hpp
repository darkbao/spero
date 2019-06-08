#ifndef SPERO_BASE_MUTEX_HPP
#define SPERO_BASE_MUTEX_HPP

#include <assert.h>
#include <pthread.h>
#include "NonCopyable.hpp"
#include "CurrentThread.h"

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function)
    noexcept __attribute__ ((__noreturn__));
__END_DECLS
#endif // NDEBUG

#define RET_CHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (UNLIKELY(errnum != 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  // CHECK_PTHREAD_RETURN_VALUE

#define RET_CHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

#endif // CHECK_PTHREAD_RETURN_VALUE

namespace spero
{

class MutexLock : public NonCopyable
{
public:
    MutexLock() : owner_(0)
    {
        RET_CHECK(pthread_mutex_init(&mutex_, NULL));
    }

    ~MutexLock()
    {
        assert(owner_ == 0);
        RET_CHECK(pthread_mutex_destroy(&mutex_));
    }

    void lock()
    {
        RET_CHECK(pthread_mutex_lock(&mutex_));
        setOwner();
    }

    void unlock()
    {
        resetOwner();
        RET_CHECK(pthread_mutex_unlock(&mutex_));
    }

    bool isLockedByThisThread() const
    {
        return owner_ == CurrentThread::tid();
    }

    void assertLocked() const
    {
        assert(isLockedByThisThread());
    }

private:
    inline pthread_mutex_t* getPthreadMutex() /* non-const */
    {
        return &mutex_;
    }

    inline void resetOwner()
    {
        owner_ = 0;
    }

    void setOwner()
    {
        owner_ = CurrentThread::tid();
    }

    class OwnerGuard : public NonCopyable  // help 'Class Condition' to reset owner's tid
    {
    public:
        explicit OwnerGuard(MutexLock& lock) : mlock_(lock)
        {
            mlock_.resetOwner();
        }

        ~OwnerGuard()
        {
            mlock_.setOwner();
        }

    private:
        MutexLock& mlock_;
    };

private:
    friend class Condition;
    pthread_mutex_t mutex_;
    pid_t owner_;
};

class MutexLockGuard : public NonCopyable
{
public:
    explicit MutexLockGuard(MutexLock& mtx) : mtxlock_(mtx)
    {
      mtxlock_.lock();  
    }

    ~MutexLockGuard()
    {
      mtxlock_.unlock();
    }

private:
    MutexLock& mtxlock_;
};

} // namespace spero

/*  
    Prevent misuse like:
        MutexLockGuard(mutex_);
    A tempory object doesn't hold the lock for long!
*/
#define MutexLockGuard(x) error "Missing guard object name"

#endif // SPERO_BASE_MUTEX_HPP