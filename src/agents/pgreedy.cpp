#include "agents/pgreedy.h"
#include "logic/simulator.h"

Rethrow PGreedy::decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int remaining) {  
    Rethrow bestRt;
    double bestProbability = 0;
    Category bestCategory = (Category)(-1);
    for(int cat = (int)Category::ACES; cat <= (int)Category::SIXES; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        auto recommended = ProbabilityEvaluator::evalGoal(hand, remaining, (Category)cat, 3);
        if(recommended.first > bestProbability) {
            bestProbability = recommended.first;
            bestCategory = (Category)cat;
            if(recommended.second) {
                bestRt = recommended.second.value();
            }
        }
    }
    for(int cat = (int)Category::FULL_HOUSE; cat <= (int)Category::YATHSEE; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        auto recommended = ProbabilityEvaluator::evalSuccess(hand, remaining, (Category)cat);
        if(recommended.first > bestProbability) {
            bestProbability = recommended.first;
            bestCategory = (Category)cat;
            if(recommended.second) {
                bestRt = recommended.second.value();
            }
        }
    }
    if(verbosity_) {
        std::cout << "Hand= " << hand.toString()
                << " Targeting " << toString(bestCategory) 
                << " with expected probability = " << bestProbability 
                << " keeping " << bestRt.toString()
                << std::endl;
    }
    return bestRt;
}

Category PGreedy::decideCategory(const Hand& hand, const ScoreCard& scoreCard) {
    double bestProbability = 0;
    Category bestCategory = Category::CHANCE;
    for(int cat = (int)Category::ACES; cat <= (int)Category::SIXES; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        auto recommended = ProbabilityEvaluator::evalGoal(hand, 0, (Category)cat, 3);
        if(recommended.first > bestProbability) {
            bestProbability = recommended.first;
            bestCategory = (Category)cat;
        }
    }
    for(int cat = (int)Category::FULL_HOUSE; cat <= (int)Category::YATHSEE; ++cat) {
        if(scoreCard.scores[cat] != ScoreCard::AVAILABLE) continue;
        auto recommended = ProbabilityEvaluator::evalSuccess(hand, 0, (Category)cat);
        if(recommended.first > bestProbability) {
            bestProbability = recommended.first;
            bestCategory = (Category)cat;
        }
    }
    if(verbosity_) {
        std::cout << "Hand= " << hand.toString()
                << " Chose " << toString(bestCategory) 
                << " with expected prob = " << bestProbability 
                << std::endl;
    }
    return bestCategory;
}