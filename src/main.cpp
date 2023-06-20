#include "logic/round.h"
#include "logic/evaluator.h"
#include "logic/simulator.h"
#include "logic/filters.h"
#include "logic/gamegraph.h"
#include "arena/arena.h"
#include "agents/random.h"
#include "agents/greedy.h"
#include "agents/smartgreedy.h"
#include "agents/bonuspriorityzing.h"
#include "agents/learner.h"
#include <iostream>
#include <cstring>

void test() {

    Hand hand = Hand::fromArray({{6, 6, 3, 4, 5}});

    std::vector<std::pair<Rethrow, PointEvaluator::eval_t>> evals;

    Simulator::forAllRethrows(hand, [&](const Hand&, const Rethrow& rt) {
        auto e = Simulator::expectedScore(hand, rt);
        evals.push_back(std::make_pair(rt, e));
    });


    std::vector<std::pair<Rethrow, PointEvaluator::eval_t>> nonDominatedEvals;
    nonDominatedEvals.resize(evals.size());
    auto end = filterDominated(evals.begin(), evals.end(), nonDominatedEvals.begin(), [](const auto& a, const auto& b) {
        return PointEvaluator::isDominatedBy(a->second, b->second);
    });
    nonDominatedEvals.resize(std::distance(nonDominatedEvals.begin(), end));

    std::cout << evals.size() << " evals | " << nonDominatedEvals.size() << " non dominated" << std::endl;
    for(const auto& e : nonDominatedEvals) {
        std::cout << "[";
        for(int i = 0; i < e.first.size; ++i) std::cout << (int)e.first.keptValues[i];
        std::cout << "]\n";
        std::cout << PointEvaluator::toString(e.second);
    }

    // size_t nbEvals = 0;
    // PointEvaluator::eval_t evalSum;
    // std::fill(evalSum.begin(), evalSum.end(), 0.0);

    // Simulator::forAllThrows(5-rt.size, [&](const Throw& t) {
    //     Hand newHand = combine(rt, t);
    //     PointEvaluator::eval_t newEval = PointEvaluator::eval(newHand);
    //     std::cout << "[";
    //     for(int i = 0; i < rt.size; ++i) std::cout << (int)rt.keptValues[i];
    //     std::cout << "]";
    //     std::cout << "[";
    //     for(int i = 0; i < t.size; ++i) std::cout << (int)t.values[i];
    //     std::cout << "]\n";
    //     ++nbEvals;
    //     for(size_t i = 0; i < newEval.size(); ++i) evalSum[i] += newEval[i];
    // });

    // forAllCategories([&](Category c) {
    //     std::cout << toString(c) << " : " << 1.0*evalSum[(int)c] / nbEvals << std::endl;
    // });

    // forAllCategories([&](Category c) {
    //     std::cout << PointEvaluator::eval(hand, c) << std::endl;
    // });

    // Simulator::forAllThrows(2, [](const Throw& t) {
    //     std::cout << t.toString() << std::endl;
    // });
}

template<typename AgentType, typename... Args>
void testAgent(const char* name, Args... args) {
    double totalScore = 0;
    int nbRounds = 100;
    for(int seed = 0; seed < nbRounds; ++seed) {
        AgentType agent(args...);
        // agent.setVerbosity(1);
        SinglePlayerArena arena;
        arena.setAgent(agent);
        arena.play(seed);
        const auto& scores = arena.scorecard();
        totalScore += scores.currentScore();
    }
    std::cout << name << " : " << totalScore/nbRounds << std::endl;
}

int main() {

    GameGraph::create();

    testAgent<Random>("random", 0);
    testAgent<Greedy>("greedy");
    testAgent<SmartGreedy>("smartgreedy");
    testAgent<BonusPriorityzing>("bonusprio");
    // testAgent<Learner>("learner");

    // for(int i = 0; i < 100; ++i) {
    //     Random a(i);
    //     SinglePlayerArena arena;
    //     arena.setAgent(a);
    //     arena.play();
    //     const auto& scores = arena.scorecard();
    //     std::cout << scores.currentScore() << std::endl;
    // }

    // for(int i = 0; i < 100; ++i) {
    //     Random a(2*i+0);
    //     Random b(2*i+1);
    //     TwoPlayerArena arena;
    //     arena.setAgents(a, b);
    //     arena.play();
    //     const auto& scores = arena.scorecards();
    //     // scores[0].display();
    //     // scores[1].display();
    //     std::cout << scores[0].currentScore() << std::endl;
    //     std::cout << scores[1].currentScore() << std::endl;
    // }
}