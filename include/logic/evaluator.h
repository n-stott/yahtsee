#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "logic/enums.h"
#include <array>

class Round;
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

};

#endif