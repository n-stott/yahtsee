#include "agents/smartergreedy.h"
#include "logic/simulator.h"
#include <numeric>

static double mean(const PointEvaluator::eval_t& eval, const ScoreCard& scoreCard) {
    double m = 0.0;
    double c = 0;
    for(int cat = 0; cat < (int)Category::size; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        m += eval[cat];
        c += 1;
    }
    return m/c;
}

Rethrow SmarterGreedy::decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int) {  
    Rethrow bestRt;
    double bestExpectedScore = -1;
    double bestExpectedMean = -1;
    Category bestCategory;
    Simulator::forAllRethrows2(hand, [&](const Hand& hand, const Rethrow& rt) {
        auto eval = Simulator::expectedScore(hand, rt);
        double meanEval = mean(eval, scoreCard);
        for(int cat = 0; cat < (int)Category::size; ++cat) {
            if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
            double value = eval[cat];
            if(value > bestExpectedScore || (value == bestExpectedScore && meanEval > bestExpectedMean)) {
                bestRt = rt;
                bestExpectedScore = value;
                bestExpectedMean = meanEval;
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

Category SmarterGreedy::decideCategory(const Hand& hand, const ScoreCard& scoreCard) {
    Category bestCategory = Category::CHANCE;
    int bestScore = -1;
    const auto& evalData = GameGraph::handEval_.at(hand.toId());
    for(int cat = 0; cat < (int)Category::size; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        int value = evalData[cat];
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