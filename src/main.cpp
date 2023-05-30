#include "logic/round.h"
#include "logic/evaluator.h"
#include "logic/simulator.h"
#include <iostream>
#include <cstring>

int main() {

    Hand hand = Hand::fromArray({{6, 6, 3, 4, 5}});

    Simulator::forAllRethrows(hand, [&](const Hand&, const Rethrow& rt) {

        size_t nbEvals = 0;
        PointEvaluator::eval_t evalSum;
        std::memset(evalSum.data(), 0, evalSum.size()*sizeof(PointEvaluator::eval_t::value_type));

        Simulator::forAllThrows(5-rt.size, [&](const Throw& t) {
            Hand newHand = combine(rt, t);
            PointEvaluator::eval_t newEval = PointEvaluator::eval(newHand);
            std::cout << "[";
            for(int i = 0; i < rt.size; ++i) std::cout << (int)rt.keptValues[i];
            std::cout << "]";
            std::cout << "[";
            for(int i = 0; i < t.size; ++i) std::cout << (int)t.values[i];
            std::cout << "]\n";
            ++nbEvals;
            for(size_t i = 0; i < newEval.size(); ++i) evalSum[i] += newEval[i];
        });

        forAllCategories([&](Category c) {
            std::cout << toString(c) << " : " << 1.0*evalSum[(int)c] / nbEvals << std::endl;
        });
    });



    // forAllCategories([&](Category c) {
    //     std::cout << PointEvaluator::eval(hand, c) << std::endl;
    // });

    // Simulator::forAllThrows(2, [](const Throw& t) {
    //     std::cout << t.toString() << std::endl;
    // });

    return 0;
}