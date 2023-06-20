#include "agents/learner.h"
#include "logic/simulator.h"
#include "arena/randomgenerator.h"
#include <unordered_set>

Learner::Learner() : Learner(0) { }

Learner::Learner(int seed) {

    std::unordered_set<HandId> allHands;

    Simulator::forAllThrows(5, [&](const Throw& t) {
        Hand h = Hand::fromArray(t.values);
        HandId hid = h.toId();
        allHands.insert(hid);
    });

    for(auto it = lookupRethrow_.begin(); it != lookupRethrow_.end(); ++it) {
        Hand hand = Hand::fromId(it->first);
        Simulator::forAllRethrows(hand, [&](const Hand& hand, const Rethrow& rt) {
            
        });
    }
}

void Learner::beginGame() {

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