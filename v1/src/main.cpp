#include "logic.h"
#include "arena.h"
#include "agents/random.h"
#include <iostream>
#include <cassert>

int main() {
    Arena arena;
    Random randomAgent(0);
    for(int seed = 0; seed < 1000000; ++seed) {
        arena.play(randomAgent, seed);
    }
}