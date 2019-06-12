#ifndef SPERO_BASE_ASYNCLOGGING_H
#define SPERO_BASE_ASYNCLOGGING_H

#include <atomic>
#include <vector>
#include <string>
#include <memory>
#include "Thread.h"
#include "CountDownLatch.h"
#include "FixedBuffer.hpp"

namespace spero
{

class AsyncLogging : public NonCopyable
{
public:
    AsyncLogging(const std::string& baseName,
                 off_t rollSize,
                 int flushInterval = 3);

    ~AsyncLogging()
    {
        if (running_)
            stop();
    }

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop()
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

    void append(const char* logline, int len);

protected:
    void threadFunc();

private:
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferVector;

    const int         flushInterval_;
    const off_t       rollSize_;
    const std::string baseName_;
    std::atomic<bool> running_;

    Thread         thread_;
    CountDownLatch latch_;
    MutexLock      mutex_;
    Condition      cond_;

    BufferPtr    currentBuffer_;
    BufferPtr    nextBuffer_;
    BufferVector buffers_;
};

} // namespace spero

#endif // SPERO_BASE_ASYNCLOGGING_H