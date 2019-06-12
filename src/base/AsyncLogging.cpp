#include <stdio.h>

#include "AsyncLogging.h"
#include "LogFile.h"
#include "Timestamp.h"

namespace spero
{

using std::string;

AsyncLogging::AsyncLogging(const string& baseName,
                           off_t rollSize, int flushInterval)
    : flushInterval_(flushInterval),
      rollSize_(rollSize),
      baseName_(baseName),
      running_(false),
      thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"),
      latch_(1),
      mutex_(),
      cond_(mutex_),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer)
{
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);
}

void AsyncLogging::threadFunc()
{
    assert(running_ == true);
    latch_.countDown();
    LogFile output(baseName_, rollSize_, false);
    BufferPtr currBackupBuffer(new Buffer);
    BufferPtr nextBackupBuffer(new Buffer);
    currBackupBuffer->bzero();
    nextBackupBuffer->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    
    while (running_) {
        assert(currBackupBuffer && currBackupBuffer->length() == 0);
        assert(nextBackupBuffer && nextBackupBuffer->length() == 0);
        assert(buffersToWrite.empty());
        
        // critical zone
        {
            MutexLockGuard lock(mutex_);
            if (buffers_.empty())
                cond_.waitForSeconds(flushInterval_);
            buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(currBackupBuffer);
            buffersToWrite.swap(buffers_);
            if (!nextBuffer_)
                nextBuffer_ = std::move(nextBackupBuffer);
        }

        assert(!buffersToWrite.empty());
        size_t n = buffersToWrite.size();
        if (n > 25) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Dropped log messages at %s, %lu large buffers\n",
                     Timestamp::now().toFormattedString().c_str(), n-2);
            fputs(buf, stderr);
            output.append(buf, static_cast<int>(strlen(buf)));
            buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
        }

        for (const auto& buffer: buffersToWrite)
            output.append(buffer->data(), buffer->length());

        if (buffersToWrite.size() > 2)
            buffersToWrite.resize(2);
        
        if (!currBackupBuffer) {
            assert(!buffersToWrite.empty());
            currBackupBuffer = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            currBackupBuffer->reset();
        }

        if (!nextBackupBuffer) {
            assert(!buffersToWrite.empty());
            nextBackupBuffer = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            nextBackupBuffer->reset();
        }

        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}

void AsyncLogging::append(const char* logline, int len)
{
    MutexLockGuard lock(mutex_);
    if (currentBuffer_->avail() > len) {
        currentBuffer_->append(logline, len);
    } else {
        buffers_.push_back(std::move(currentBuffer_));
        if (nextBuffer_)
            currentBuffer_ = std::move(nextBuffer_);
        else
            currentBuffer_.reset(new Buffer);
        currentBuffer_->append(logline, len);
        cond_.notify();
    }
}


} // namespace spero