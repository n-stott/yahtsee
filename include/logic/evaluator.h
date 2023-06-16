#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "logic/enums.h"
#include <array>
#include <string>

class Hand;

class ProbabilityEvaluator {
public:
    using eval_t = std::array<double, (int)Category::size>;

    static eval_t eval(const Hand& hand, int remainingThrows);

private:

};

class PointEvaluator {
public:
    using eval_t = std::array<double, (int)Category::size>;
    
    static int eval(const Hand& hand, Category category);
    static eval_t eval(const Hand& hand);

    static bool isDominatedBy(const eval_t& a, const eval_t& b) {
        bool dominates = true;
        forAllCategories([&](Category c) {
            if(c == Category::CHANCE) return;
            dominates &= a[(int)c] <= b[(int)c];
        });
        return dominates;
    }

    static std::string toString(const eval_t& e);

};

#endif