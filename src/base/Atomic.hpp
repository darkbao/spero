#ifndef SPERO_BASE_ATOMIC_HPP
#define SPERO_BASE_ATOMIC_HPP

#include <stdint.h>
#include "NonCopyable.hpp"

namespace spero
{

template <typename T>
class AtomicInteger : public NonCopyable
{
public:
    AtomicInteger() : value_(0)
    { }
    
    T get()
    {
        // in gcc >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x)
    {
        // in gcc >= 4.7: __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST)
        // __sync_fetch_and_add(&i, 1) --> i++
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x)
    {
        // __sync_add_and_fetch(&i, 1) --> ++i
        // return __sync_add_and_fetch(&value_, x);
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T newValue)
    {
        // in gcc >= 4.7: __atomic_exchange_n(&value, newValue, __ATOMIC_SEQ_CST)
        return __sync_lock_test_and_set(&value_, newValue);
    }

private:
    volatile T value_;
};

typedef AtomicInteger<int32_t> AtomicInt32;
typedef AtomicInteger<int64_t> AtomicInt64;

} // namespace spero

#endif // SPERO_BASE_ATOMIC_HPP