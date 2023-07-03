#include "logic.h"
#include "arena.h"
#include "agents/random.h"
#include <chrono>

int main() {
    Arena arena;
    Random randomAgent(0);
    auto begin = std::chrono::steady_clock::now();
    for(int seed = 0; seed < 1000000; ++seed) {
        arena.play(randomAgent, seed);
    }
    auto end = std::chrono::steady_clock::now();
#ifndef NDEBUG
    if(std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() >= 5) return 1;
#else
    if(std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() >= 2) return 1;
#endif
    return 0;
}