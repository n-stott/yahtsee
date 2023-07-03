#include "agents/random.h"

Kept Random::decideKept(const Hand&, const ScoreCard&, int) {
    unsigned int mask = qr_.next(0, 31);
    return Kept { mask };
}

Category Random::decideCategory(const Hand&, const ScoreCard& scoreCard) {
    return (Category)qr_.nextInMask(scoreCard.availabilityMask());
}

void Random::endGame(const ScoreCard&) { }