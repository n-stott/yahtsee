#ifndef SMARTERGREEDY_H
#define SMARTERGREEDY_H

#include "agents/agent.h"

class SmarterGreedy : public Agent {
public:

    Rethrow decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) override;
    Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) override;

};

#endif