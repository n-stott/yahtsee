#ifndef GREEDY_H
#define GREEDY_H

#include "agents/agent.h"

class Greedy : public Agent {
public:

    Rethrow decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) override;
    Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) override;

};

#endif