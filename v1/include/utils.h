#ifndef UTILS_H
#define UTILS_H

#include <cassert>

class Quickrand {
public:
    explicit Quickrand(int seed) : seed_(seed) { }

    int next(int a, int b) {
        assert(a <= b);
        return a + fastrand() % (b-a+1);
    }

    int nextInMask(unsigned int mask) {
        assert(mask != 0);
        int count = __builtin_popcount(mask);
        int choice = next(0, count-1);
        int p = -1;
        for(int i = 0; i < 32; ++i) {
            if(mask & (1 << i)) p += 1;
            if(p == choice) {
                return i;
            }
        }
        assert(false);
        return 0;
    }

private:

    int seed_;

    int fastrand() {
        seed_ = 214013*seed_ + 2531011;
        return (seed_ >> 16) & 0x7FFF;
    } 

};

#endif