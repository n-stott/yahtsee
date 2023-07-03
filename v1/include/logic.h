#ifndef LOGIC_H
#define LOGIC_H

#include <array>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Hand {
public:
    explicit Hand(int, int, int, int, int);
    explicit Hand(int dice[]);

    int operator[](int d) const;

    const std::array<int, 5>& values() const;

    unsigned int id() const;

    unsigned int hash() const;

    std::string toString() const;

    template<typename Func>
    static void forAllHands(Func func) {
        if(handCache_.empty()) createCache();
        for(size_t id = 0; id < handCache_.size(); ++id) func(Hand::fromId(id));
    }

private:
    unsigned int id_;

    explicit Hand() = default;

    static unsigned int hash(const int dice[]);
    static unsigned int maxId();

    unsigned int computeHash() const;

    friend class GameGraph;

    static void createCache();

    static Hand fromId(unsigned int id);
    static Hand fromHash(unsigned int hash);

    static std::vector<std::array<int, 5>> handCache_;
    static std::vector<unsigned int> hashCache_;
    static std::unordered_map<unsigned int, unsigned int> hashToId_;
};

class Kept {
public:
    explicit Kept(unsigned int mask);

    int size() const;
    unsigned int mask() const;

private:
    unsigned int mask_;
};

enum Category {
    ACES,
    TWOS,
    THREES,
    FOURS,
    FIVES,
    SIXES,
    THREE_OF_A_KIND,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    SMALL_STRAIGHT,
    LARGE_STRAIGHT,
    YAHTSEE,
    CHANCE,
    NB_CATEGORIES,
};

class ScoreCard {
public:
    explicit ScoreCard();

    bool isAvailable(Category category) const;
    int score(Category category) const;
    bool hasBonus() const;

    static int eval(Category category, Hand hand);

    bool write(Category category, int score);

    unsigned int availabilityMask() const;

private:
    std::array<int, NB_CATEGORIES> scores_;
    int familiesScore_;
    unsigned int availabilityMask_;

    static std::array<int, NB_CATEGORIES> internalEval(Hand hand);

    static void createCache();

    static std::vector<std::array<int, NB_CATEGORIES>> evalCache_;
};

class GameGraph {
public:
    static std::unique_ptr<GameGraph> tryCreate();

    const std::vector<Kept>& possibleKept(Hand hand) const;

    const std::vector<std::pair<Hand, int>>& futures(Hand hand, Kept kept) const;

private:
    GameGraph();

    std::vector<std::vector<Kept>> possibleKept_;

    std::vector<std::vector<std::vector<std::pair<Hand, int>>>> edges_;
};

#endif