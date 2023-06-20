#ifndef LEARNER_H
#define LEARNER_H

#include "agents/agent.h"
#include <unordered_map>

struct HandId;
struct ScoreCardId;
struct RethrowId;

class Learner : public Agent {
public:

    Learner();
    explicit Learner(int seed);

    void beginGame() override;
    void endGame(int score) override;

    Rethrow decideRethrow(const Hand& hand, const ScoreCard& scoreCard, int remainingThrows) override;
    Category decideCategory(const Hand& hand, const ScoreCard& scoreCard) override;

private:
    template<typename K, typename V>
    using map = std::unordered_map<K, V>;

    map<HandId, map<Category, std::vector<std::pair<RethrowId, double>>>> lookupRethrow_;
    map<HandId, map<Category, std::vector<std::pair<Category, double>>>> lookupCategory_;

    map<HandId, std::vector<RethrowId>> allRethrows_;
};

#endif