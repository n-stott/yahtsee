#ifndef ARENA_H
#define ARENA_H

#include "logic.h"

class Agent;

class Arena {
public:
    ScoreCard play(Agent& agent, int seed);

private:
};

#endif