#include "agents/smartgreedy.h"
#include "logic/simulator.h"

Rethrow SmartGreedy::decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int) {  
    Rethrow bestRtNoChance;
    double bestExpectedScoreNoChance = -1;
    Rethrow bestRtChance;
    double bestExpectedScoreChance = -1;
    Category bestCategory = Category::CHANCE;
    Simulator::forAllRethrows(hand, [&](const Hand& hand, const Rethrow& rt) {
        auto eval = Simulator::expectedScore(hand, rt);
        for(int cat = 0; cat < (int)Category::size; ++cat) {
            if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
            if(cat == (int)Category::CHANCE) {
                double value = eval[cat];
                if(value > bestExpectedScoreChance) {
                    bestRtChance = rt;
                    bestExpectedScoreChance = value;
                }
            } else {
                double value = eval[cat];
                if(value > bestExpectedScoreNoChance) {
                    bestRtNoChance = rt;
                    bestExpectedScoreNoChance = value;
                    bestCategory = (Category)cat;
                }
            }
        }
    });

    Rethrow bestRt = (bestExpectedScoreNoChance != -1) ? bestRtNoChance : bestRtChance;
    double bestExpectedScore = (bestExpectedScoreNoChance != -1) ? bestExpectedScoreNoChance : bestExpectedScoreChance;
    if(verbosity_) {
        std::cout << "Hand= " << hand.toString()
                << " Targeting " << toString(bestCategory) 
                << " with expected score = " << bestExpectedScore 
                << " keeping " << bestRt.toString()
                << std::endl;
    }
    return bestRt;
}

Category SmartGreedy::decideCategory(const Hand& hand, const ScoreCard& scoreCard) {
    Category bestCategory = Category::CHANCE;
    int bestScore = -1;
    const auto& evalData = PointEvaluator::lookupEvaluation(hand.toId());
    for(int cat = 0; cat < (int)Category::size; ++cat) {
        if(cat == (int)Category::CHANCE) continue;
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        int value = evalData.evaluation[cat];
        if(value > bestScore) {
            bestCategory = (Category)cat;
            bestScore = value;
        }
    }
    assert(bestScore >= 0 || bestCategory == Category::CHANCE);
    if(verbosity_) {
        std::cout << "Hand= " << hand.toString()
                << " Chose " << toString(bestCategory) 
                << " with expected score = " << bestScore 
                << std::endl;
    }
    return bestCategory;
}