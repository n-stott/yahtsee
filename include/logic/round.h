#ifndef ROUND_H
#define ROUND_H

#include "logic/enums.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
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

class ScoreCard {
public:
    ScoreCard() {
        std::fill(scores.begin(), scores.end(), AVAILABLE);
    }

    void write(Category cat, int points) {
        assert(scores[(int)cat] == AVAILABLE);
        scores[(int)cat] = points;
    }

    int currentScore() const {
        int score = 0;
        for(int v : scores) score += (v >= 0 ? v : 0);
        score += bonus();
        return score;
    }

    int bonus() const {
        int value = 0;
        value += scores[(int)Category::ACES];
        value += scores[(int)Category::TWOS];
        value += scores[(int)Category::THREES];
        value += scores[(int)Category::FOURS];
        value += scores[(int)Category::FIVES];
        value += scores[(int)Category::SIXES];
        return (value >= 63) ? 35 : 0;
    }

    void display() const {
        forAllCategories([&](Category c) {
            std::cout << toString(c) << " : " << scores[(int)c] << std::endl;
        });
    }

    static constexpr int AVAILABLE = -1;

    std::array<int, (int)Category::size> scores;
};


#endif