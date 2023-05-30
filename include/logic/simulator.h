#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "logic/round.h"
#include "utils.h"
#include <cmath>
#include <iostream>

class Simulator {
public:

    static PointEvaluator::eval_t expectedScore(const Hand& hand, const Rethrow& rt) {
        PointEvaluator::eval_t evalSum;
        std::fill(evalSum.begin(), evalSum.end(), 0.0);

        size_t nbEvals = 0;
        Simulator::forAllThrows(5-rt.size, [&](const Throw& t) {
            Hand newHand = combine(rt, t);
            PointEvaluator::eval_t newEval = PointEvaluator::eval(newHand);
            ++nbEvals;
            for(size_t i = 0; i < newEval.size(); ++i) evalSum[i] += newEval[i];
        });

        if(nbEvals != 0) {
            for(double& sc : evalSum) sc /= nbEvals;
        }

        return evalSum;
    }

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