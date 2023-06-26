#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "logic/evaluator.h"
#include "logic/gamegraph.h"
#include "logic/round.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <unordered_map>

class Simulator {
public:

    static double expectedScore(const Hand& hand, const Rethrow& rt, Category cat) {
        double evalSum = 0.0;

        const auto& futures = GameGraph::graph_.at(hand.toId()).at(rt.toId());

        size_t nbEvals = 0;
        for(const auto& e : futures) {
            nbEvals += e.second;
            const auto& evalData = GameGraph::handEval_.at(e.first);
            evalSum += e.second * evalData[(int)cat];
        }
        if(nbEvals != 0) evalSum /= nbEvals;
        return evalSum;
    }

    static PointEvaluator::eval_t expectedScore(const Hand& hand, const Rethrow& rt) {
        PointEvaluator::eval_t evalSum;
        std::fill(evalSum.begin(), evalSum.end(), 0.0);

        const auto& futures = GameGraph::graph_.at(hand.toId()).at(rt.toId());

        size_t nbEvals = 0;
        for(const auto& e : futures) {
            nbEvals += e.second;
            const auto& evalData = GameGraph::handEval_.at(e.first);
            for(size_t i = 0; i < evalData.size(); ++i) evalSum[i] += e.second * evalData[i];
        }

        if(nbEvals != 0) {
            for(double& sc : evalSum) sc /= nbEvals;
        }

        return evalSum;
    }
    
    static PointEvaluator::eval_t expectedSuccess(const Hand& hand, const Rethrow& rt) {
        PointEvaluator::eval_t evalSum;
        std::fill(evalSum.begin(), evalSum.end(), 0.0);

        const auto& futures = GameGraph::graph_.at(hand.toId()).at(rt.toId());

        size_t nbEvals = 0;
        for(const auto& e : futures) {
            nbEvals += e.second;
            const auto& evalData = GameGraph::handEval_.at(e.first);
            for(size_t i = 0; i < evalData.size(); ++i) evalSum[i] += e.second * (evalData[i] > 0);
        }

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
    static void forAllRethrows2(const Hand& hand, Callback&& callback) {
        HandId hid = hand.toId();
        assert(rethrows_.contains(hid));
        const auto& rts = rethrows_[hid];
        for(const Rethrow& rt : rts) callback(hand, rt);
    }

    static int fastPow(int base, int exp) {
        int value = 1;
        while(exp--) value *= base;
        return value;
    }

    template<typename Callback>
    static void forAllThrows(u8 size, Callback&& callback) {
        for(int mask = 0; mask < fastPow(6, size); ++mask) {
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

private:
    static std::unordered_map<HandId, std::vector<Rethrow>> rethrows_;

};

#endif