// use gdb to find buffer's data from coredump file
#include <stdio.h>
#include "FixedBuffer.hpp"

using namespace spero;

int main()
{
    FixedBuffer<kLargeBuffer> buffer;
    printf("avail len: %lu\n", buffer.avail());
    const char* str = "Hello, world\n";
    size_t len = strlen(str) + 1;
    buffer.append(str, len);
    printf("appended: %lu, avail: %lu\n", len, buffer.avail());
    int a = 0;
    int b = 100/a;
    printf("never get here, b:%d\n", b);
    return 0;
}