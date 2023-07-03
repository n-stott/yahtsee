#include "agents/random.h"
#include <iostream>

Kept Random::decideKept(const Hand& hand, const ScoreCard&, int) {
    unsigned int mask = qr_.next(0, 31);
    return Kept { mask };
}

Category Random::decideCategory(const Hand&, const ScoreCard& scoreCard) {
    return (Category)qr_.nextInMask(scoreCard.availabilityMask());
}

void Random::endGame(const ScoreCard& scorecard) {
    // std::cout << scorecard.currentScore() << std::endl;
}