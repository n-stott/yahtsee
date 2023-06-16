#include "agents/greedy.h"
#include "logic/simulator.h"

Rethrow Greedy::decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int) {
    Rethrow bestRt;
    double bestExpectedScore = -1;
    Simulator::forAllRethrows(hand, [&](const Hand& hand, const Rethrow& rt) {
        auto eval = Simulator::expectedScore(hand, rt);
        for(int cat = 0; cat < (int)Category::size; ++cat) {
            if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
            double value = eval[cat];
            if(value > bestExpectedScore) {
                bestRt = rt;
                bestExpectedScore = value;
            }
        }
    });
    return bestRt;
}

Category Greedy::decideCategory(const Hand& hand, const ScoreCard& scoreCard) {
    Category bestCategory = Category::CHANCE;
    int bestScore = -1;
    const auto& evalData = PointEvaluator::lookupEvaluation(hand);
    for(int cat = 0; cat < (int)Category::size; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        int value = evalData.evaluation[cat];
        if(value > bestScore) {
            bestCategory = (Category)cat;
            bestScore = value;
        }
    }
    assert(bestScore >= 0);
    return bestCategory;
}