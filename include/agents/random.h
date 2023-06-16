#ifndef RANDOM_H
#define RANDOM_H

#include "agents/agent.h"
#include "arena/randomgenerator.h"

class Random : public Agent {
public:
    explicit Random(int seed) : rg_(seed) { }

    Rethrow decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) override;
    Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) override;

private:
    RandomGenerator rg_;
};

#endif