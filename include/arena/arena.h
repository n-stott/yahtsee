#ifndef ARENA_H
#define ARENA_H

#include "logic/round.h"
#include <array>

class Agent;

class SinglePlayerArena {
public:
    SinglePlayerArena() = default;
    
    void setAgent(Agent& a);

    void play();

    const ScoreCard& scorecard() const { return scorecard_; }

private:
    Agent* agent_ = nullptr;
    ScoreCard scorecard_;
};

class TwoPlayerArena {
public:
    TwoPlayerArena() = default;
    
    void setAgents(Agent& a, Agent& b);

    void play();

    const std::array<ScoreCard, 2>& scorecards() const { return scorecards_; }

private:
    std::array<Agent*, 2> agents_ { nullptr, nullptr };
    std::array<ScoreCard, 2> scorecards_;
};

#endif