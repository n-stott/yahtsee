#include "logic.h"
#include "arena.h"
#include "agents/random.h"
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
    Arena arena;
    Random randomAgent(0);
    arena.setAgent(randomAgent);
    for(int seed = 0; seed < 1000000; ++seed) {
        arena.play(seed);
    }
}