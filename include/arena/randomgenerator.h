#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>

class RandomGenerator {
public:
    explicit RandomGenerator(int seed) : rng(seed) { }

    int next(int lo, int up) {
        std::uniform_int_distribution<std::mt19937::result_type> dist(lo, up);
        return dist(rng);
    }

    int next() { return next(1, 6); }

private:
    std::mt19937 rng;

};

#endif