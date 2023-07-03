#include "arena.h"
#include "logic.h"
#include "agents/agent.h"
#include "utils.h"
#include <array>

Hand throwAllDice(Quickrand& qr) {
    return Hand::randomHand(qr);
}

Hand rethrowSomeDice(Hand hand, Kept kept, Quickrand& qr) {
    return GameGraph::the()->randomHandAfterKept(hand, kept, qr);
}


void Arena::setAgent(Agent& a) {
    agent_ = &a;
}

void Arena::play(int seed) {
    scorecard_ = ScoreCard{};
    if(!agent_) return;
    Quickrand qr(seed);
    agent_->beginGame();
    for(int round = 0; round < NB_CATEGORIES; ++round) {
        Agent* agent = agent_;
        ScoreCard& scorecard = scorecard_;
        Hand hand = throwAllDice(qr);
        Kept kept = agent->decideKept(hand, scorecard, 2);
        if(kept.size() != 5) {
            hand = rethrowSomeDice(hand, kept, qr);
            kept = agent->decideKept(hand, scorecard, 1);
            if(kept.size() != 5) {
                hand = rethrowSomeDice(hand, kept, qr);
            }
        }
        Category category = agent->decideCategory(hand, scorecard);
        int score = ScoreCard::eval(category, hand);
        bool success = scorecard.write(category, score);
    }
    agent_->endGame(scorecard_);
}