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

    auto gg = GameGraph::tryCreate();

}