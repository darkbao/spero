#ifndef SPERO_BASE_CURRENTTHREAD_H
#define SPERO_BASE_CURRENTTHREAD_H

#include <stdint.h>
#include <string>

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

namespace spero
{

namespace CurrentThread
{
    extern __thread int  t_cachedTid;
    extern __thread int  t_tidStringLength;
    extern __thread char t_tidString[32];
    extern __thread const char* t_threadName;
    
    void cacheTid();
    bool isMainThread();
    void sleepUsec(int64_t usec);
    std::string stackTrace(bool demangle);

    inline int tid()
    {
        if (UNLIKELY(t_cachedTid == 0)) {
            cacheTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString()
    {
        return t_tidString;
    }

    inline int tidStringLength()
    {
        return t_tidStringLength;
    }

    inline const char* name()
    {
        return t_threadName;
    }

} // namspace CurrentThread
} // namespace spero

#endif // SPERO_BASE_CURRENTTHREAD_H