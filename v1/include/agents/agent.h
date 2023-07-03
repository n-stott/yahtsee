#ifndef AGENT_H
#define AGENT_H

#include "logic.h"

class Agent {
public:
    virtual ~Agent() = default;

    virtual void beginGame() { };
    
    virtual void endGame(const ScoreCard&) { };
    
    virtual Kept decideKept(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) = 0;

    virtual Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) = 0;

    void setVerbosity(int level) { verbosity_ = level; }

protected:
    int verbosity_ = 0;
};

#endif