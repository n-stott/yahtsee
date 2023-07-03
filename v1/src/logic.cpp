#include "logic.h"
#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>

std::vector<std::array<int, 5>> Hand::handCache_;
std::vector<unsigned int> Hand::allHandsById_;
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
    assert(id < maxId());
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

Hand Hand::randomHand(Quickrand& qr) {
    if(allHandsById_.empty()) {
        for(int a = 1; a <= 6; ++a) {
            for(int b = 1; b <= 6; ++b) {
                for(int c = 1; c <= 6; ++c) {
                    for(int d = 1; d <= 6; ++d) {
                        for(int e = 1; e <= 6; ++e) {
                            std::array<int, 5> dice {{ a, b, c, d, e }};
                            unsigned int hash = Hand::hash(dice.data());
                            Hand hand = Hand::fromHash(hash);
                            allHandsById_.push_back(hand.id());
                        }
                    }
                }
            }
        }
    }
    unsigned int pos = qr.next(0, allHandsById_.size()-1);
    return Hand::fromId(allHandsById_[pos]);
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
    for(size_t id = 0; id < handCache_.size(); ++id) {
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

bool Kept::isSet(unsigned int pos) const {
    assert(pos < 5);
    return mask_ & (1 << pos);
}

std::string Kept::toString() const {
    std::string s;
    for(int i = 0; i < 5; ++i) {
        s += (mask_ & (1 << i)) ? "1" : "0";
    }
    return s;
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

int ScoreCard::currentScore() const {
    return totalScore_ + hasBonus() * 35;
}

int ScoreCard::eval(Category category, Hand hand) {
    if(evalCache_.empty()) createCache();
    return evalCache_[hand.id()][category];
}

const std::array<int, NB_CATEGORIES>& ScoreCard::eval(Hand hand) {
    if(evalCache_.empty()) createCache();
    return evalCache_[hand.id()];
}

bool ScoreCard::write(Category category, int score) {
    assert(category >= 0 && category < NB_CATEGORIES);
    if(!isAvailable(category)) return false;
    scores_[category] = score;
    availabilityMask_ &= ~(1 << category);
    if(category <= SIXES) familiesScore_ += score;
    totalScore_ += score;
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
    score[SMALL_STRAIGHT] = 30 * ((occurrences[3] >= 1 && occurrences[4] >= 1)
                                 && ((occurrences[1] >= 1 && occurrences[2] >= 1) 
                                 || (occurrences[2] >= 1 && occurrences[5] >= 1) 
                                 || (occurrences[5] >= 1 && occurrences[6] >= 1)));
    score[LARGE_STRAIGHT] = 40 * ((occurrences[2] >= 1 && occurrences[3] >= 1 && occurrences[4] >= 1 && occurrences[5] >= 1)
                                 && (occurrences[1] >= 1 || occurrences[6] >= 1));
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

Hand GameGraph::randomHandAfterKept(Hand hand, Kept kept, Quickrand& qr) const {
    assert(hand.id() < rawEdges_.size());
    assert(kept.mask() < rawEdges_[hand.id()].size());
    const auto& rfuts = rawEdges_[hand.id()][kept.mask()];
    int pos = qr.next(0, rfuts.size()-1);
    return rfuts[pos];
}

GameGraph::GameGraph() = default;

std::unique_ptr<GameGraph> GameGraph::create() {
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
    std::vector<std::vector<std::vector<Hand>>> rawEdges;
    std::vector<int> counts;
    counts.resize(Hand::maxId(), 0);
    Hand::forAllHands([&](Hand hand) {
        std::vector<std::vector<std::pair<Hand, int>>> es;
        std::vector<std::vector<Hand>> res;
        for(int m = 0; m < 32; ++m) {
            std::vector<Hand> rfuts;
            std::array<int, 5> vals;
            int size = 0;
            for(int i = 0; i < 5; ++i) if (m & (1 << i)) vals[size++] = hand[i];
            std::fill(counts.begin(), counts.end(), 0);
            if(size == 5) {
                rfuts.push_back(hand);
                counts[hand.id()] += 1;
            } else if (size == 4) {
                for(const auto& rt : rt1) {
                    std::array<int, 5> dice {{ vals[0], vals[1], vals[2], vals[3], rt[0] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    rfuts.push_back(h);
                    counts[h.id()] += 1;
                }
            } else if (size == 3) {
                for(const auto& rt : rt2) {
                    std::array<int, 5> dice {{ vals[0], vals[1], vals[2], rt[0], rt[1] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    rfuts.push_back(h);
                    counts[h.id()] += 1;
                }
            } else if (size == 2) {
                for(const auto& rt : rt3) {
                    std::array<int, 5> dice {{ vals[0], vals[1], rt[0], rt[1], rt[2] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    rfuts.push_back(h);
                    counts[h.id()] += 1;
                }
            } else if (size == 1) {
                for(const auto& rt : rt4) {
                    std::array<int, 5> dice {{ vals[0], rt[0], rt[1], rt[2], rt[3] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    rfuts.push_back(h);
                    counts[h.id()] += 1;
                }
            } else if (size == 0) {
                for(const auto& rt : rt5) {
                    std::array<int, 5> dice {{ rt[0], rt[1], rt[2], rt[3], rt[4] }};
                    int hash = Hand::hash(dice.data());
                    Hand h = Hand::fromHash(hash);
                    rfuts.push_back(h);
                    counts[h.id()] += 1;
                }
            }
            std::vector<std::pair<Hand, int>> futs;
            for(size_t i = 0; i < counts.size(); ++i) {
                if(counts[i] == 0) continue;
                futs.push_back(std::make_pair(Hand::fromId(i), counts[i]));
            }
            es.push_back(std::move(futs));
            res.push_back(std::move(rfuts));
        }
        edges.push_back(std::move(es));
        rawEdges.push_back(std::move(res));
    });

    auto gg = std::unique_ptr<GameGraph>(new GameGraph());
    std::swap(gg->possibleKept_, possibleKept);
    std::swap(gg->edges_, edges);
    std::swap(gg->rawEdges_, rawEdges);
    return gg;
}

const GameGraph* GameGraph::the() {
    static auto gg = GameGraph::create();
    return gg.get();
}

std::vector<std::vector<std::vector<std::array<double, NB_CATEGORIES>>>> EvaluationGraph::expectedScoreCache_;

const std::array<int, NB_CATEGORIES>& EvaluationGraph::score(Hand hand) const {
    return ScoreCard::eval(hand);
}

const std::array<double, NB_CATEGORIES>& EvaluationGraph::expectedScore(unsigned int categoryMask, Hand hand, Kept kept) const {
    if(expectedScoreCache_.empty()) createCache();
    categoryMask &= ((1 << NB_CATEGORIES) - 1); // mask out unused bits
    if(expectedScoreCache_[categoryMask].empty()) createCache(categoryMask);
    return expectedScoreCache_[categoryMask][hand.id()][kept.mask()];
}

void EvaluationGraph::createCache() {
    expectedScoreCache_.resize(1 << NB_CATEGORIES);
}

void EvaluationGraph::createCache(unsigned int categoryMask) {
    std::vector<std::vector<std::array<double, NB_CATEGORIES>>> expectedScoreCache;
    const GameGraph* gg = GameGraph::the();
    std::vector<int> categories;
    for(int category = 0; category < NB_CATEGORIES; ++category) {
        if(categoryMask & (1 << category)) categories.push_back(category);
    }
    Hand::forAllHands([&](Hand hand) {
        std::vector<std::array<double, NB_CATEGORIES>> es;
        es.reserve(gg->possibleKept(hand).size());
        for(Kept kept : gg->possibleKept(hand)) {
            std::array<double, NB_CATEGORIES> ss;
            size_t count = 0;
            std::fill(ss.begin(), ss.end(), 0.0);
            for(const auto& d : gg->futures(hand, kept)) {
                const auto& eval = ScoreCard::eval(d.first);
                for(int category : categories) {
                    ss[category] += d.second * eval[category];
                }
                count += d.second;
            }
            std::for_each(ss.begin(), ss.end(), [&](auto& v) { v /= count; });
            es.push_back(ss);
        }
        expectedScoreCache.push_back(std::move(es));
    });
    std::swap(expectedScoreCache_[categoryMask], expectedScoreCache);
}