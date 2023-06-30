#ifndef BONUSPRIORITYZING_H
#define BONUSPRIORITYZING_H

#include "agents/agent.h"

class BonusPriorityzing : public Agent {
public:

    Rethrow decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) override;
    Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) override;

};

#endif