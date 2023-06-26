#ifndef LEARNER_H
#define LEARNER_H

#include "agents/agent.h"
#include "arena/randomgenerator.h"
#include <unordered_map>
#include <vector>

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

    map<int, map<HandId, map<ScoreCardId, RethrowId>>> r;
    map<HandId, map<ScoreCardId, Category>> d;

    struct Roll {
        int remaining;
        HandId hid;
        ScoreCardId scid;
        RethrowId rid;
    };

    struct Decision {
        HandId hid;
        ScoreCardId scid;
        Category cat;
    };

    std::vector<Roll> rolls_;
    std::vector<Decision> decisions_;

    RandomGenerator rng;
};

#endif