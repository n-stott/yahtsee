#include "agents/random.h"

Rethrow Random::decideRethrow(const Hand& hand, const ScoreCard&, int) {
    Rethrow rt;
    for(int v : hand.values) {
        bool keepValue = rg_.next(0, 1);
        if(!keepValue) continue;
        rt.keptValues[rt.size++] = v;
    }
    return rt;
}

Category Random::decideCategory(const Hand&, const ScoreCard& scoreCard) {
    int nbAvailableCategories = std::count(scoreCard.scores.begin(), scoreCard.scores.end(), ScoreCard::AVAILABLE);
    assert(nbAvailableCategories > 0);
    int choice = rg_.next(0, nbAvailableCategories-1);
    int p = -1;
    for(int i = 0; i < (int)Category::size; ++i) {
        if(scoreCard.scores[i] == ScoreCard::AVAILABLE) ++p;
        if(p == choice) return (Category)i;
    }
    assert(!"not reachable");
    return Category::CHANCE;
}