#ifndef SMARTGREEDY_H
#define SMARTGREEDY_H

#include "agents/agent.h"

class SmartGreedy : public Agent {
public:

    Rethrow decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) override;
    Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) override;

};

#endif