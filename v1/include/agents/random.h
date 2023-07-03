#ifndef RANDOM_H
#define RANDOM_H

#include "agents/agent.h"
#include "utils.h"

class Random : public Agent {
public:
    explicit Random(int seed) : qr_(seed) { }

    void endGame(const ScoreCard& scorecard) override;

    Kept decideKept(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) override;
    Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) override;

private:
    Quickrand qr_;
};

#endif