#include "logic.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>

std::vector<std::array<int, 5>> Hand::handCache_;
std::vector<unsigned int> Hand::hashCache_;
std::unordered_map<unsigned int, unsigned int> Hand::hashToId_;

Hand::Hand(int a, int b, int c, int d, int e) {
    std::array<int, 5> dice {{ a, b, c, d, e }};
    std::sort(dice.begin(), dice.end());
    if(handCache_.empty()) createCache();
    auto it = std::lower_bound(handCache_.begin(), handCache_.end(), dice);
    assert(*it == dice);
    id_ = std::distance(handCache_.begin(), it);
}

Hand::Hand(int ds[]) {
    std::array<int, 5> dice {{ ds[0], ds[1], ds[2], ds[3], ds[4] }};
    std::sort(dice.begin(), dice.end());
    if(handCache_.empty()) createCache();
    auto it = std::lower_bound(handCache_.begin(), handCache_.end(), dice);
    assert(*it == dice);
    id_ = std::distance(handCache_.begin(), it);
}

Hand Hand::fromId(unsigned int id) {
    Hand hand;
    hand.id_ = id;
    return hand;
}

int Hand::operator[](int d) const {
    assert(d >= 0 && d < 5);
    assert(!handCache_.empty());
    return handCache_[id_][d];
}

const std::array<int, 5>& Hand::values() const {
    assert(!handCache_.empty());
    return handCache_[id_];
}

unsigned int Hand::id() const {
    return id_;
}

unsigned int Hand::maxId() {
    if(handCache_.empty()) createCache();
    return handCache_.size();
}

unsigned int Hand::hash(const int dice[]) {
    const int pows[7] = { 1, 10, 100, 1000, 10000, 100000, 1000000 };
    unsigned int h = 5555555;
    for(int k = 0; k < 5; ++k) {
        h -= pows[7-dice[k]];
    }
    return h;
}

unsigned int Hand::computeHash() const {
    assert(!handCache_.empty());
    const auto& vals = values();
    return Hand::hash(vals.data());
}

unsigned int Hand::hash() const {
    assert(!hashCache_.empty());
    return hashCache_[id_];
}

Hand Hand::fromHash(unsigned int hash) {
    if(hashToId_.empty()) createCache();
    assert(hashToId_.contains(hash));
    return Hand::fromId(hashToId_[hash]);
}

std::string Hand::toString() const {
    std::string s;
    s += std::to_string(this->operator[](0)) + " ";
    s += std::to_string(this->operator[](1)) + " ";
    s += std::to_string(this->operator[](2)) + " ";
    s += std::to_string(this->operator[](3)) + " ";
    s += std::to_string(this->operator[](4));
    return s;
}

void Hand::createCache() {
    std::vector<std::array<int, 5>> cache;
    for(int a = 1; a <= 6; ++a) {
        for(int b = a; b <= 6; ++b) {
            for(int c = b; c <= 6; ++c) {
                for(int d = c; d <= 6; ++d) {
                    for(int e = d; e <= 6; ++e) {
                        std::array<int, 5> dice {{ a, b, c, d, e }};
                        cache.push_back(dice);
                    }
                }
            }
        }
    }
    auto it = std::unique(cache.begin(), cache.end());
    cache.erase(it, cache.end());
    std::swap(handCache_, cache);

    std::vector<unsigned int> hashCache;
    hashCache.reserve(handCache_.size());
    for(size_t id = 0; id < handCache_.size(); ++id) {
        Hand h = Hand::fromId(id);
        hashCache.push_back(h.computeHash());
    }
    std::swap(hashCache_, hashCache);

    std::unordered_map<unsigned int, unsigned int> hashToId;
    for(size_t id = 0; id < cache.size(); ++id) {
        Hand h = Hand::fromId(id);
        hashToId[h.hash()] = id;
    }
    std::swap(hashToId_, hashToId);
}

Kept::Kept(unsigned int mask) : mask_(mask) { }

int Kept::size() const {
    return __builtin_popcount(mask_);
}

unsigned int Kept::mask() const {
    return mask_;
}

std::vector<std::array<int, NB_CATEGORIES>> ScoreCard::evalCache_;

ScoreCard::ScoreCard() {
    std::fill(scores_.begin(), scores_.end(), 0);
    familiesScore_ = 0;
    availabilityMask_ = (1 << NB_CATEGORIES) - 1;
}

bool ScoreCard::isAvailable(Category category) const {
    assert(category >= 0 && category < NB_CATEGORIES);
    return availabilityMask_ & (1 << category);
}

int ScoreCard::score(Category category) const {
    return scores_[category];
}

bool ScoreCard::hasBonus() const {
    return familiesScore_ >= 63;
}

int ScoreCard::eval(Category category, Hand hand) {
    if(evalCache_.empty()) createCache();
    return evalCache_[hand.id()][category];
}

bool ScoreCard::write(Category category, int score) {
    assert(category >= 0 && category < NB_CATEGORIES);
    if(!isAvailable(category)) return false;
    scores_[category] = score;
    availabilityMask_ &= ~(1 << category);
    if(category <= SIXES) familiesScore_ += score;
    return true;
}

unsigned int ScoreCard::availabilityMask() const {
    return availabilityMask_;
}

std::array<int, NB_CATEGORIES> ScoreCard::internalEval(Hand hand) {
    std::array<int, 7> occurrences { 0, 0, 0, 0, 0, 0, 0 };
    int sum = 0;
    for(int v : hand.values()) {
        ++occurrences[v];
        sum += v;
    }

    std::array<int, NB_CATEGORIES> score;
    const int INVALID = -1;
    std::fill(score.begin(), score.end(), INVALID);

    score[ACES]   = occurrences[1] * 1;
    score[TWOS]   = occurrences[2] * 2;
    score[THREES] = occurrences[3] * 3;
    score[FOURS]  = occurrences[4] * 4;
    score[FIVES]  = occurrences[5] * 5;
    score[SIXES]  = occurrences[6] * 6;

    int maxOccurence = 0;
    int nbDuos = 0;
    int nbTrios = 0;
    for(int o : occurrences) {
        maxOccurence = std::max(maxOccurence, o);
        nbDuos += (o == 2);
        nbTrios += (o == 3);
    }
    score[THREE_OF_A_KIND] = (maxOccurence >= 3) * sum;
    score[FOUR_OF_A_KIND] = (maxOccurence >= 4) * sum;
    score[FULL_HOUSE] = 25 * (nbDuos == 1 && nbTrios == 1);
    score[SMALL_STRAIGHT] = (occurrences[3] >= 1 && occurrences[4] >= 1) 
                            && ((occurrences[1] >= 1 && occurrences[2] >= 1) 
                             || (occurrences[2] >= 1 && occurrences[5] >= 1) 
                             || (occurrences[5] >= 1 && occurrences[6] >= 1));
    score[LARGE_STRAIGHT] = (occurrences[2] >= 1 && occurrences[3] >= 1 && occurrences[4] >= 1 && occurrences[5] >= 1)
                            && (occurrences[1] >= 1 || occurrences[6] >= 1);
    score[YAHTSEE] = 50 * (maxOccurence == 5);
    score[CHANCE] = sum;

    assert(std::count(score.begin(), score.end(), INVALID) == 0);

    return score;
}

void ScoreCard::createCache() {
    std::vector<std::array<int, NB_CATEGORIES>> cache;
    Hand::forAllHands([&](Hand hand) {
        std::array<int, NB_CATEGORIES> evals = internalEval(hand);
        cache.push_back(evals);
    });
    std::swap(evalCache_, cache);
}

const std::vector<Kept>& GameGraph::possibleKept(Hand hand) const {
    assert(hand.id() < possibleKept_.size());
    return possibleKept_[hand.id()];
}

const std::vector<std::pair<Hand, int>>& GameGraph::futures(Hand hand, Kept kept) const {
    assert(hand.id() < edges_.size());
    assert(kept.mask() < edges_[hand.id()].size());
    return edges_[hand.id()][kept.mask()];
}

GameGraph::GameGraph() = default;

std::unique_ptr<GameGraph> GameGraph::tryCreate() {
    std::vector<std::vector<Kept>> possibleKept;
    Hand::forAllHands([&](Hand hand) {
        std::map<unsigned int, unsigned int> map;
        for(unsigned int m = 0; m < 32; ++m) {
            std::array<int, 5> v { 0, 0, 0, 0, 0 };
            int size = 0;
            for(int i = 0; i < 5; ++i) {
                if(m & (1 << i)) v[size++] = hand[i];
            }
            unsigned int hash = 0;
            for(int i = 0; i < size; ++i) {
                hash = 10*hash + v[i];
            }
            map[hash] = m;
        }
        std::vector<Kept> distinctKept;
        for(auto it = map.begin(); it != map.end(); ++it) distinctKept.push_back(Kept(it->second));
        possibleKept.push_back(distinctKept);
    });

    std::vector<std::array<int, 1>> rt1;
    std::vector<std::array<int, 2>> rt2;
    std::vector<std::array<int, 3>> rt3;
    std::vector<std::array<int, 4>> rt4;
    std::vector<std::array<int, 5>> rt5;
    for(int a = 1; a <= 6; ++a) {
        rt1.push_back({ a });
        for(int b = 1; b <= 6; ++b) {
            rt2.push_back({ a, b });
            for(int c = 1; c <= 6; ++c) {
                rt3.push_back({ a, b, c });
                for(int d = 1; d <= 6; ++d) {
                    rt4.push_back({ a, b, c, d });
                    for(int e = 1; e <= 6; ++e) {
                        rt5.push_back({ a, b, c, d, e });
                    }
                }
            }
        }
    }

    std::vector<std::vector<std::vector<std::pair<Hand, int>>>> edges;
    std::vector<int> counts;
    counts.resize(Hand::maxId(), 0);
    Hand::forAllHands([&](Hand hand) {
        std::vector<std::vector<std::pair<Hand, int>>> es;
        for(int m = 0; m < 32; ++m) {
            std::array<int, 5> vals;
            int size = 0;
            for(int i = 0; i < 5; ++i) if (m & (1 << i)) vals[size++] = hand[i];
            std::fill(counts.begin(), counts.end(), 0);
            if(size == 5) {
                counts[hand.id()] += 1;
            } else if (size == 4) {
                for(const auto& rt : rt1) {
                    std::array<int, 5> dice {{ vals[0], vals[1], vals[2], vals[3], rt[0] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    counts[h.id()] += 1;
                }
            } else if (size == 3) {
                for(const auto& rt : rt2) {
                    std::array<int, 5> dice {{ vals[0], vals[1], vals[2], rt[0], rt[1] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    counts[h.id()] += 1;
                }
            } else if (size == 2) {
                for(const auto& rt : rt3) {
                    std::array<int, 5> dice {{ vals[0], vals[1], rt[0], rt[1], rt[2] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    counts[h.id()] += 1;
                }
            } else if (size == 1) {
                for(const auto& rt : rt4) {
                    std::array<int, 5> dice {{ vals[0], rt[0], rt[1], rt[2], rt[3] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    counts[h.id()] += 1;
                }
            } else if (size == 0) {
                for(const auto& rt : rt5) {
                    std::array<int, 5> dice {{ rt[0], rt[1], rt[2], rt[3], rt[4] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    counts[h.id()] += 1;
                }
            }
            std::vector<std::pair<Hand, int>> futs2;
            for(size_t i = 0; i < counts.size(); ++i) {
                if(counts[i] == 0) continue;
                futs2.push_back(std::make_pair(Hand::fromId(i), counts[i]));
            }
            es.push_back(futs2);
        }
        edges.push_back(es);
    });

    auto gg = std::unique_ptr<GameGraph>(new GameGraph());
    std::swap(gg->possibleKept_, possibleKept);
    std::swap(gg->edges_, edges);
    return gg;
}