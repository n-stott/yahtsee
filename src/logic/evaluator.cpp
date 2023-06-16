#include "logic/evaluator.h"
#include "logic/round.h"
#include "logic/enums.h"
#include <algorithm>
#include <cassert>
#include <numeric>

std::array<double, (int)Category::size> ProbabilityEvaluator::eval(const Hand& hand, int remainingThrows) {
    std::array<double, (int)Category::size> ps;

    return ps;
}

int PointEvaluator::eval(const Hand& hand, Category category) {
    assert(std::is_sorted(hand.values.begin(), hand.values.end()));
    switch(category) {
        case Category::ACES:   return std::count(hand.values.begin(), hand.values.end(), 1) * 1;
        case Category::TWOS:   return std::count(hand.values.begin(), hand.values.end(), 2) * 2;
        case Category::THREES: return std::count(hand.values.begin(), hand.values.end(), 3) * 3;
        case Category::FOURS:  return std::count(hand.values.begin(), hand.values.end(), 4) * 4;
        case Category::FIVES:  return std::count(hand.values.begin(), hand.values.end(), 5) * 5;
        case Category::SIXES:  return std::count(hand.values.begin(), hand.values.end(), 6) * 6;
        case Category::THREE_OF_A_KIND: {
            const auto& values = hand.values;
            if(values[0] == values[2]) return 3*values[0];
            if(values[1] == values[3]) return 3*values[1];
            if(values[2] == values[4]) return 3*values[2];
            return 0;
        }
        case Category::FOUR_OF_A_KIND: {
            const auto& values = hand.values;
            if(values[0] == values[3]) return 4*values[0];
            if(values[1] == values[4]) return 4*values[1];
            return 0;
        }
        case Category::FULL_HOUSE: {
            const auto& values = hand.values;
            if(values[0] == values[1] && values[2] == values[4]) return 25;
            if(values[0] == values[2] && values[3] == values[4]) return 25;
            return 0;
        }
        case Category::SMALL_STRAIGHT: {
            const auto& values = hand.values;
            if(values[1]+1 != values[2]) return 0;
            if(values[2]+1 != values[3]) return 0;
            if(values[0]+1 == values[1]) return 30;
            if(values[3]+1 == values[4]) return 30;
            return 0;
        }
        case Category::LARGE_STRAIGHT: {
            const auto& values = hand.values;
            if(values[0]+1 != values[1]) return 0;
            if(values[1]+1 != values[2]) return 0;
            if(values[2]+1 != values[3]) return 0;
            if(values[3]+1 != values[4]) return 0;
            return 40;
        }
        case Category::YATHSEE: {
            const auto& values = hand.values;
            if(values[0] == values[4]) return 50;
            return 0;
        }
        case Category::CHANCE: {
            return std::accumulate(hand.values.begin(), hand.values.end(), 0);
        }
    }
    assert(false);
    __builtin_unreachable();
}

PointEvaluator::eval_t PointEvaluator::eval(const Hand& hand) {
    eval_t ps;
    for(int cat = 0; cat < (int)Category::size; ++cat) {
        ps[cat] = eval(hand, (Category)cat);
    }
    return ps;
}


std::string PointEvaluator::toString(const eval_t& e) {
    std::string s;
    forAllCategories([&](Category c) {
        s += ::toString(c);
        s += " ";
        s += std::to_string(e[(int)c]);
        s += '\n';
    });
    return s;
}