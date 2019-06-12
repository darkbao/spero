#ifndef SPERO_BASE_FIXEDBUFFER_HPP
#define SPERO_BASE_FIXEDBUFFER_HPP

#include <string>
#include <algorithm>
#include "StringPiece.hpp"

namespace spero
{

const size_t kSmallBuffer = 4096;
const size_t kLargeBuffer = 4096*1024;

template <size_t SIZE>
class FixedBuffer : public NonCopyable
{
public:
    FixedBuffer() : cur_(data_), avail_(SIZE)
    {
        setCookie(cookieStart);
    }

    ~FixedBuffer()
    {
        setCookie(cookieEnd);
    }

    size_t append(const char* buf, size_t len)
    {
        len = std::min(avail_, len);
        memcpy(cur_, buf, len);
        cur_   += len;
        avail_ -= len;
        return len;
    }

    inline const char* data() const
    { 
        return data_;
    }
    inline size_t length() const
    { 
        return (SIZE - avail_);
    }

    // write to data_ directly
    inline char*  current()
    { 
        return cur_;
    }
    inline size_t avail() const
    { 
        return avail_;
    }
    inline void   add(size_t len)
    { 
        cur_ += len;
    }

    inline void reset()
    { 
        cur_ = data_;
    }
    inline void bzero()
    { 
        memset(data_, 0, sizeof(data_));
    }

    // for used by GDB
    const char* debugString()
    {
        *cur_ = '\0';
        return data_;
    }
    void setCookie(void (*cookie)())
    {
        cookie_ = cookie;
    }
    
    // for used by unit test
    std::string toString() const
    { 
        return std::string(data_, length());
    }
    StringPiece toStringPiece() const
    { 
        return StringPiece(data_, length());
    }

private:
    static void cookieStart() { }
    static void cookieEnd() { }

private:
    void (*cookie_)();
    char     data_[SIZE];
    char*    cur_;
    size_t   avail_;
};

} // namespace spero

#endif // SPERO_BASE_FIXEDBUFFER_HPP