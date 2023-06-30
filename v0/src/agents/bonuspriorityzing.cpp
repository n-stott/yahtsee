#include "agents/bonuspriorityzing.h"
#include "logic/simulator.h"

Rethrow BonusPriorityzing::decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int) {  
    Rethrow bestRt;
    double bestExpectedScore = -1;
    Category bestCategory;
    bool hasAvailableFamily = scoreCard.hasAvailableFamily();
    Simulator::forAllRethrows(hand, [&](const Hand& hand, const Rethrow& rt) {
        auto eval = Simulator::expectedScore(hand, rt);
        int begin = hasAvailableFamily ? (int)Category::ACES : (int)Category::THREE_OF_A_KIND;
        int end = hasAvailableFamily ? (int)Category::THREE_OF_A_KIND : (int)Category::size;
        for(int cat = begin; cat < end; ++cat) {
            if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
            double value = eval[cat];
            if(value > bestExpectedScore) {
                bestRt = rt;
                bestExpectedScore = value;
                bestCategory = (Category)cat;
            }
        }
    });
    if(verbosity_) {
        std::cout << "Hand= " << hand.toString()
                << " Targeting " << toString(bestCategory) 
                << " with expected score = " << bestExpectedScore 
                << " keeping " << bestRt.toString()
                << std::endl;
    }
    return bestRt;
}

Category BonusPriorityzing::decideCategory(const Hand& hand, const ScoreCard& scoreCard) {
    Category bestCategory = Category::CHANCE;
    int bestScore = -1;
    const auto& evalData = PointEvaluator::lookupEvaluation(hand.toId());
    bool hasAvailableFamily = scoreCard.hasAvailableFamily();
    int begin = hasAvailableFamily ? (int)Category::ACES : (int)Category::THREE_OF_A_KIND;
    int end = hasAvailableFamily ? (int)Category::THREE_OF_A_KIND : (int)Category::size;
    for(int cat = begin; cat < end; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        int value = evalData.evaluation[cat];
        if(value > bestScore) {
            bestCategory = (Category)cat;
            bestScore = value;
        }
    }
    assert(bestScore >= 0);
    if(verbosity_) {
        std::cout << "Hand= " << hand.toString()
                << " Chose " << toString(bestCategory) 
                << " with expected score = " << bestScore 
                << std::endl;
    }
    return bestCategory;
}