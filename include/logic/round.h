#ifndef ROUND_H
#define ROUND_H

#include "logic/enums.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>

struct HandId {
    u64 id;

    friend bool operator==(const HandId& a, const HandId& b) { return a.id == b.id; }
};

struct Hand {
    std::array<u8, 5> values;

    std::string toString() const {
        std::string s;
        for(auto v : values) s += std::to_string((int)v)  + " ";
        return s;
    }

    static Hand fromArray(std::array<u8, 5> values) {
        Hand hand;
        hand.values = values;
        hand.sort();
        return hand;
    }

    static Hand fromId(HandId id) {
        Hand hand;
        hand.values[4] = id.id % 6 + 1;
        id.id /= 6;
        hand.values[3] = id.id % 6 + 1;
        id.id /= 6;
        hand.values[2] = id.id % 6 + 1;
        id.id /= 6;
        hand.values[1] = id.id % 6 + 1;
        id.id /= 6;
        hand.values[0] = id.id % 6 + 1;
        return hand;
    }

    HandId toId() const {
        u64 id = 0;
        for(int v : values) id = 6*id + (v-1);
        return HandId{id};
    }

    void sort() {
        std::sort(values.begin(), values.end());
    }

    friend bool operator==(const Hand& a, const Hand& b) {
        return a.values == b.values;
    }
};

struct ThrowId {
    u64 id;

    friend bool operator==(const ThrowId& a, const ThrowId& b) { return a.id == b.id; }
};

struct Throw {
    std::array<u8, 5> values {{ 0, 0, 0, 0, 0 }};
    u8 size = 0;

    std::string toString() const {
        std::string s;
        for(u8 i = 0; i < size; ++i) s += std::to_string((int)values[i]);
        return s;
    }

    static Throw fromId(ThrowId id) {
        Throw t;
        t.values[0] = id.id % 6 + 1;
        id.id /= 6;
        t.values[1] = id.id % 6 + 1;
        id.id /= 6;
        t.values[2] = id.id % 6 + 1;
        id.id /= 6;
        t.values[3] = id.id % 6 + 1;
        id.id /= 6;
        t.values[4] = id.id % 6 + 1;
        return t;
    }

    ThrowId toId() const {
        u64 id = 0;
        for(int i = size; i --> 0;) {
            id = 6*id + (values[i]-1);
        }
        return ThrowId{id};
    }
};

struct RethrowId {
    u64 id;

    friend bool operator==(const RethrowId& a, const RethrowId& b) { return a.id == b.id; }
};

struct Rethrow {
    std::array<u8, 5> keptValues {{ 0, 0, 0, 0, 0 }};
    u8 size = 0;

    static Rethrow fromId(RethrowId id) {
        Rethrow rt;
        // std::cout << id.id << " " << (int)rt.size << std::endl;
        rt.keptValues[0] = id.id % 7;
        id.id /= 7;
        rt.keptValues[1] = id.id % 7;
        id.id /= 7;
        rt.keptValues[2] = id.id % 7;
        id.id /= 7;
        rt.keptValues[3] = id.id % 7;
        id.id /= 7;
        rt.keptValues[4] = id.id % 7;
        for(rt.size = 0; rt.size < 5; ++rt.size) {
            if(rt.keptValues[rt.size] == 0) break;
        }
        // std::cout << id.id << " " << (int)rt.size << std::endl;
        return rt;
    }

    RethrowId toId() const {
        u64 id = 0;
        for(int i = size; i --> 0;) {
            id = 7*id + keptValues[i];
        }
        return RethrowId{id};
    }

    std::string toString() const {
        std::string s;
        for(int i = 0; i < size; ++i) s += std::to_string((int)keptValues[i])  + " ";
        return s;
    }
};

inline Hand combine(const Rethrow& rt, const Throw& t) {
    Hand h;
    u8 p = 0;
    assert(rt.size + t.size == 5);
    for(u8 i = 0; i < rt.size; ++i) h.values[p++] = rt.keptValues[i];
    for(u8 i = 0; i < t.size; ++i) h.values[p++] = t.values[i];
    std::sort(h.values.begin(), h.values.end());
    return h;
}

struct ScoreCardId {
    u64 id;

    bool isAvailable(Category cat) const {
        return id & (1 << (int)cat);
    }

    friend bool operator==(const ScoreCardId& a, const ScoreCardId& b) { return a.id == b.id; }
};

class ScoreCard {
public:
    ScoreCard() {
        std::fill(scores.begin(), scores.end(), AVAILABLE);
    }

    ScoreCardId toId() const {
        u64 mask = 0;
        for(int i = 0; i < (int)Category::size; ++i) {
            if(scores[i] == AVAILABLE) mask |= (1 << i);
        }
        return ScoreCardId{mask};
    }

    void write(Category cat, int points) {
        assert(scores[(int)cat] == AVAILABLE);
        scores[(int)cat] = points;
    }

    int currentScore() const {
        int score = 0;
        for(int v : scores) score += (v >= 0 ? v : 0);
        score += bonus();
        return score;
    }

    int bonus() const {
        int value = 0;
        value += scores[(int)Category::ACES];
        value += scores[(int)Category::TWOS];
        value += scores[(int)Category::THREES];
        value += scores[(int)Category::FOURS];
        value += scores[(int)Category::FIVES];
        value += scores[(int)Category::SIXES];
        return (value >= 63) ? 35 : 0;
    }

    void display() const {
        forAllCategories([&](Category c) {
            std::cout << toString(c) << " : " << scores[(int)c] << std::endl;
        });
    }

    bool hasAvailableFamily() const {
        for(int cat = (int)Category::ACES; cat <= (int)Category::SIXES; ++cat) {
            if(scores[cat] == AVAILABLE) return true;
        }
        return false;
    }

    static constexpr int AVAILABLE = -1;

    std::array<int, (int)Category::size> scores;
};



namespace std {
    template<>
    struct hash<Hand> {
        auto operator()(const Hand& hand) const -> size_t {
            return hand.toId().id;
        }
    };
    
    template<>
    struct hash<HandId> {
        auto operator()(const HandId& id) const -> size_t {
            return id.id;
        }
    };

    template<>
    struct hash<RethrowId> {
        auto operator()(const RethrowId& id) const -> size_t {
            return id.id;
        }
    };

    template<>
    struct hash<ScoreCardId> {
        auto operator()(const ScoreCardId& id) const -> size_t {
            return id.id;
        }
    };
}


#endif
