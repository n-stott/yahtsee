#include "logic.h"
#include <iostream>
#include <cassert>

void testHandConsistency() {
#ifndef NDEBUG
    Hand::forAllHands([](Hand hand1) {
        Hand::forAllHands([&](Hand hand2) {
            bool io = (hand1.id() < hand2.id());
            bool ho = (hand1.hash() < hand2.hash());
            assert(io == ho);
        });
    });
#endif
}

int main() {

    Hand hand { 1, 1, 2, 3, 4 };
    Kept kept(0b01111);
    EvaluationGraph eg;
    for(unsigned int i = 0; i < (1 << NB_CATEGORIES); ++i) {
        std::cout << i << std::endl;
        auto e = eg.expectedScore(i, hand, kept);
    }
    // for(double d : e) {
    //     std::cout << d << std::endl;
    // }


}