#include <vector>
#include <stdio.h>
#include "Timestamp.h"

using spero::Timestamp;
using std::vector;

void passByValue(Timestamp ts)
{
    printf("%s\n", ts.toString().c_str());
}

void passByConstReference(const Timestamp& ts)
{
    printf("%s\n", ts.toString().c_str());
}

void benchmark()
{
    const int kNum = 1000*1000;
    vector<Timestamp> stamps;
    stamps.reserve(kNum);
    for (int i = 0; i < kNum; ++i) {
        stamps.push_back(Timestamp::now());
    }
    printf("front:%s\n", stamps.front().toString().c_str());
    printf("back :%s\n", stamps.back().toString().c_str());
    printf("diff :%f\n", timeDiff(stamps.back(), stamps.front()));
    
    int increments[101] = { 0 };
    int64_t start = stamps.front().microSecondsSinceEpoch();
    for (int i = 1; i < kNum; ++i) {
        int64_t next = stamps[i].microSecondsSinceEpoch();
        int64_t inc = next - start;
        start = next;
        if (inc < 0) {
            printf("reverse!\n");
        } else if (inc < 100) {
            ++increments[inc];
        } else {
            printf("big gap:%d, index:%d\n", static_cast<int>(inc), i);
            ++increments[100];
        }
    }

    printf("stat result:\n");
    printf(">100: %d\n", increments[100]);
    for (int i = 0; i < 100; ++i) {
        printf("%2d: %d\n", i, increments[i]);
    }
}

int main(int argc, char const *argv[])
{
    Timestamp now(Timestamp::now());
    printf("%s\n", now.toString().c_str());
    passByValue(now);
    passByConstReference(now);
    benchmark();
    return 0;
}