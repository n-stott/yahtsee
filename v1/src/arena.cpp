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

ScoreCard Arena::play(Agent& agent, int seed) {
    ScoreCard scorecard;
    Quickrand qr(seed);
    agent.beginGame();
    for(int round = 0; round < NB_CATEGORIES; ++round) {
        agent.beginRound(scorecard);
        Hand hand = throwAllDice(qr);
        Kept kept = agent.decideKept(hand, scorecard, 2);
        hand = rethrowSomeDice(hand, kept, qr);
        kept = agent.decideKept(hand, scorecard, 1);
        hand = rethrowSomeDice(hand, kept, qr);
        Category category = agent.decideCategory(hand, scorecard);
        int score = ScoreCard::eval(category, hand);
        bool success = scorecard.write(category, score);
        if(success) {
            agent.reportValidCategory(scorecard, category);
        } else {
            agent.reportInvalidCategory(scorecard, category);
        }
        agent.endRound(scorecard);
    }
    agent.endGame(scorecard);
    return scorecard;
}