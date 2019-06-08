#ifndef SPERO_BASE_NONCOPYABLE_HPP
#define SPERO_BASE_NONCOPYABLE_HPP

namespace spero
{

class NonCopyable
{

public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    
};

}

#endif // SPERO_BASE_NONCOPYABLE_HPP