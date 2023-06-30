#include "arena/arena.h"
#include "arena/randomgenerator.h"
#include "logic/enums.h"
#include "logic/round.h"
#include "logic/evaluator.h"
#include "agents/agent.h"

Hand throwAllDice(RandomGenerator& rg) {
    Hand h;
    for(u8& v : h.values) v = rg.next();
    h.sort();
    return h;
}

Throw throwSomeDice(RandomGenerator& rg, int count) {
    Throw t;
    for(int i = 0; i < count; ++i) {
        t.values[t.size++] = rg.next();
    }
    return t;
}


void SinglePlayerArena::setAgent(Agent& a) {
    agent_ = &a;
}

void SinglePlayerArena::play(int seed) {
    scorecard_ = ScoreCard{};
    if(!agent_) return;
    RandomGenerator rg(seed);
    agent_->beginGame();
    for(int round = 0; round < (int)Category::size; ++round) {
        Agent* agent = agent_;
        ScoreCard& scorecard = scorecard_;
        Hand hand = throwAllDice(rg);
        Rethrow rt = agent->decideRethrow(hand, scorecard, 2);
        if(rt.size != 0) {
            Throw t = throwSomeDice(rg, 5-rt.size);
            hand = combine(rt, t);
            rt = agent->decideRethrow(hand, scorecard, 1);
            if(rt.size != 0) {
                Throw t = throwSomeDice(rg, 5-rt.size);
                hand = combine(rt, t);
            }
        }
        Category category = agent->decideCategory(hand, scorecard);
        scorecard.write(category, PointEvaluator::eval(hand, category));
    }
    agent_->endGame(scorecard_.currentScore());
}

void TwoPlayerArena::setAgents(Agent& a, Agent& b) {
    agents_ = {{ &a, &b }};
}

void TwoPlayerArena::play(int seed) {
    scorecards_ = {{ ScoreCard{}, ScoreCard{} }};
    if(agents_.empty()) return;
    RandomGenerator rg(seed);
    for(int round = 0; round < (int)Category::size; ++round) {
        for(int a = 0; a < 2; ++a) {
            Agent* agent = agents_[a];
            ScoreCard& scorecard = scorecards_[a];
            Hand hand = throwAllDice(rg);
            Rethrow rt = agent->decideRethrow(hand, scorecard, 2);
            if(rt.size != 0) {
                Throw t = throwSomeDice(rg, 5-rt.size);
                hand = combine(rt, t);
                rt = agent->decideRethrow(hand, scorecard, 1);
                if(rt.size != 0) {
                    Throw t = throwSomeDice(rg, 5-rt.size);
                    hand = combine(rt, t);
                }
            }
            Category category = agent->decideCategory(hand, scorecard);
            scorecard.write(category, PointEvaluator::eval(hand, category));
        }
    }
}