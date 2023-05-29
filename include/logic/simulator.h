#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "logic/round.h"
#include "utils.h"
#include <cmath>
#include <iostream>

class Simulator {
public:

    template<typename Callback>
    static void forAllRethrows(const Hand& hand, Callback&& callback) {
        for(int mask = 0; mask < 32; ++mask) {
            Rethrow rt;
            for(u8 i = 0; i < 5; ++i) {
                if(mask & (1 << i)) {
                    rt.keptValues[rt.size] = hand.values[i];
                    ++rt.size;
                }
            }
            callback(hand, rt);
        }
    }

    template<typename Callback>
    static void forAllThrows(u8 size, Callback&& callback) {
        for(int mask = 0; mask < std::pow(6, size); ++mask) {
            Throw t;
            int m = mask;
            for(u8 i = 0; i < size; ++i) {
                t.values[t.size] = (m % 6) + 1;
                ++t.size;
                m /= 6;
            }
            callback(t);
        }
    }

};

#endif