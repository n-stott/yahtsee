#include "agents/learner.h"
#include "logic/simulator.h"
#include "arena/randomgenerator.h"
#include <unordered_set>

Learner::Learner() : Learner(0) { }

Learner::Learner(int seed) : rng(seed) {

}

void Learner::beginGame() {
    rolls_.clear();
    decisions_.clear();
}

void Learner::endGame(int score) {
    (void)score;
}

Rethrow Learner::decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int) {
    return Rethrow::fromId(RethrowId{0});
}

Category Learner::decideCategory(const Hand& hand, const ScoreCard& scoreCard) {
    return (Category)0;
}