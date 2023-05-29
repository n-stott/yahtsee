#ifndef ROUND_H
#define ROUND_H

#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <string>

struct Hand {
    std::array<u8, 5> values;

    static Hand fromArray(std::array<u8, 5> values) {
        Hand hand;
        hand.values = values;
        hand.sort();
        return hand;
    }

    void sort() {
        std::sort(values.begin(), values.end());
    }
};

struct Throw {
    std::array<u8, 5> values {{ 0, 0, 0, 0, 0 }};
    u8 size = 0;

    std::string toString() const {
        std::string s;
        for(u8 i = 0; i < size; ++i) s += std::to_string((int)values[i]);
        return s;
    }
};

struct Rethrow {
    std::array<u8, 5> keptValues {{ 0, 0, 0, 0, 0 }};
    u8 size = 0;
};

inline Hand combine(const Rethrow& rt, const Throw& t) {
    Hand h;
    u8 p = 0;
    assert(rt.size + t.size == 5);
    for(u8 i = 0; i < rt.size; ++i) h.values[p++] = rt.keptValues[i];
    for(u8 i = 0; i < t.size; ++i) h.values[p++] = t.values[i];
    std::sort(h.values.begin(), h.values.end());
    return h;
}

struct Round {
    Hand hand;
    u8 remainingThrows;
};

#endif