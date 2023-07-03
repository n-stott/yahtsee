#ifndef ARENA_H
#define ARENA_H

#include "logic.h"

class Agent;

class Arena {
public:
    Arena() = default;
    
    void setAgent(Agent& a);

    void play(int seed);

    const ScoreCard& scorecard() const { return scorecard_; }

private:
    Agent* agent_ = nullptr;
    ScoreCard scorecard_;
};

#endif