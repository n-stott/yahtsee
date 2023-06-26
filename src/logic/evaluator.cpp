#include "logic/evaluator.h"
#include "logic/round.h"
#include "logic/enums.h"
#include "logic/simulator.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

ProbabilityEvaluator::Type ProbabilityEvaluator::typeOfCategory(Category cat) {
    switch(cat) {
        case Category::ACES: return Type::GOAL;
        case Category::TWOS: return Type::GOAL;
        case Category::THREES: return Type::GOAL;
        case Category::FOURS: return Type::GOAL;
        case Category::FIVES: return Type::GOAL;
        case Category::SIXES: return Type::GOAL;
        case Category::THREE_OF_A_KIND: return Type::SUCCESS;
        case Category::FOUR_OF_A_KIND: return Type::SUCCESS;
        case Category::FULL_HOUSE: return Type::SUCCESS;
        case Category::SMALL_STRAIGHT: return Type::SUCCESS;
        case Category::LARGE_STRAIGHT: return Type::SUCCESS;
        case Category::YATHSEE: return Type::SUCCESS;
        case Category::CHANCE: return Type::CHANCE;
        default:
            assert(false);
            return Type::CHANCE;
    }
}

std::pair<double, std::optional<Rethrow>> ProbabilityEvaluator::evalGoal(const Hand& hand, int remainingThrows, Category category, int goalCount) {
    int count = [&]() -> int {
        switch(category) {
            case Category::ACES:   return std::count(hand.values.begin(), hand.values.end(), 1);
            case Category::TWOS:   return std::count(hand.values.begin(), hand.values.end(), 2);
            case Category::THREES: return std::count(hand.values.begin(), hand.values.end(), 3);
            case Category::FOURS:  return std::count(hand.values.begin(), hand.values.end(), 4);
            case Category::FIVES:  return std::count(hand.values.begin(), hand.values.end(), 5);
            case Category::SIXES:  return std::count(hand.values.begin(), hand.values.end(), 6);
            default: {
                assert(false);
                return 0;
            }
        }
    }();

    auto factorial = [](int n) -> int {
        int res = 1;
        while(n) {
            res *= n;
            --n;
        }
        return res;
    };

    auto nChooseK = [=](int n, int k) -> int {
        assert(n >= 0);
        assert(k >= 0);
        assert(k <= n);
        return factorial(n) / (factorial(k) * factorial(n-k));
    };

    auto oneShotProbability = [=](int n, int k) -> double {
        return nChooseK(n, k) * std::pow(1.0/6.0, k) * std::pow(5.0/6.0, n-k);
    };

    auto keepCategory = [&]() {
        Rethrow rt;
        for(int i : hand.values) {
            if(i == (int)category+1) rt.keptValues[rt.size++] = i;
        }
        return rt;
    };

    assert(goalCount <= 5);
    if(count >= goalCount) {
        return std::make_pair(1.0, keepCategory());
    }
    int target = (goalCount - count);
    int dice = (5 - count);

    if(remainingThrows == 0) {
        return std::make_pair(count >= goalCount, keepCategory());
    } else if (remainingThrows == 1) {
        double p = 0.0;
        for(int t = target; t <= dice; ++t) p += oneShotProbability(dice, t);
        assert(p <= 1.0);
        Rethrow rt;
        for(int i : hand.values) {
            if(i == (int)category+1) rt.keptValues[rt.size++] = i;
        }
        return std::make_pair(p, rt);
    } else if (remainingThrows == 2) {
        double p = 0.0;
        for(int first = 0; first <= dice; ++first) {
            double q = oneShotProbability(dice, first);
            if(first >= target) {
                p += q;
            } else {
                for(int second = 0; second <= dice-first; ++second) {
                    if(first + second < target) continue;
                    p += q * oneShotProbability(dice-first, second);
                }
            }
        }
        assert(p <= 1.0);
        Rethrow rt;
        for(int i : hand.values) {
            if(i == (int)category+1) rt.keptValues[rt.size++] = i;
        }
        return std::make_pair(p, rt);
    }
    assert(false);
    return std::make_pair(0, std::nullopt);
}

std::pair<double, std::optional<Rethrow>> ProbabilityEvaluator::evalSuccess(const Hand& hand, int remainingThrows, Category category) {

    auto expectedScores = [](const Hand& hand, Category category) {
        int value = [=]() {
            switch(category) {
                case Category::FULL_HOUSE: return 25;
                case Category::SMALL_STRAIGHT: return 30;
                case Category::LARGE_STRAIGHT: return 40;
                case Category::YATHSEE: return 50;
                default: {
                    assert(false);
                    return 0;
                }
            }
        }();
        std::vector<std::pair<Rethrow, double>> res;
        Simulator::forAllRethrows2(hand, [&](const Hand& hand, const Rethrow& rt) {
            double p = Simulator::expectedScore(hand, rt, category) / value;
            if(p == 0) return;
            res.push_back(std::make_pair(rt, p));
        });
        double bestExpectedScore = 0.0;
        for(const auto& e : res) bestExpectedScore = std::max(bestExpectedScore, e.second);
        res.erase(std::remove_if(res.begin(), res.end(), [&](const auto& e) { return e.second < bestExpectedScore; }), res.end());
        return res;
    };

    static std::array<
                std::unordered_map<HandId, std::vector<std::pair<Rethrow, double>>>,
                (int)Category::size> cache;

    auto expectedScoresWithCache = [&](const Hand& hand, Category category) {
        auto& entry = cache[int(category)];
        auto it = entry.find(hand.toId());
        if(it == entry.end()) {
            auto value = expectedScores(hand, category);
            auto pit = entry.emplace(std::make_pair(hand.toId(), value));
            return pit.first->second;
        } else {
            return it->second;
        }
    };

    if(remainingThrows == 0) {
        return std::make_pair(PointEvaluator::eval(hand, category) != 0, std::nullopt);
    } else if (remainingThrows == 1) {
        auto es = expectedScoresWithCache(hand, category);
        if(es.empty()) return std::make_pair(0.0, std::nullopt);
        return std::make_pair(es[0].second, es[0].first);
    } else if (remainingThrows == 2) {
        std::vector<std::pair<Rethrow, double>> res;
        Simulator::forAllRethrows2(hand, [&](const Hand& hand, const Rethrow& rt) {
            double p = 0.0;
            int count = 0;
            Simulator::forAllThrows(5-rt.size, [&](const Throw& t) {
                Hand h = combine(rt, t);
                auto es = expectedScoresWithCache(h, category);
                ++count;
                p += es.empty() ? 0.0 : es[0].second;
            });
            res.push_back(std::make_pair(rt, count > 0 ? p / count : 0.0));
        });
        double bestExpectedScore = 0.0;
        for(const auto& e : res) bestExpectedScore = std::max(bestExpectedScore, e.second);
        res.erase(std::remove_if(res.begin(), res.end(), [&](const auto& e) { return e.second < bestExpectedScore; }), res.end());
        if(res.empty()) return std::make_pair(0.0, std::nullopt);
        return std::make_pair(res[0].second, res[0].first);
    }
    return std::make_pair(0.0, std::nullopt);
}

int PointEvaluator::eval(const Hand& hand, Category category) {
    // assert(std::is_sorted(hand.values.begin(), hand.values.end()));
    switch(category) {
        case Category::ACES:   return std::count(hand.values.begin(), hand.values.end(), 1) * 1;
        case Category::TWOS:   return std::count(hand.values.begin(), hand.values.end(), 2) * 2;
        case Category::THREES: return std::count(hand.values.begin(), hand.values.end(), 3) * 3;
        case Category::FOURS:  return std::count(hand.values.begin(), hand.values.end(), 4) * 4;
        case Category::FIVES:  return std::count(hand.values.begin(), hand.values.end(), 5) * 5;
        case Category::SIXES:  return std::count(hand.values.begin(), hand.values.end(), 6) * 6;
        case Category::THREE_OF_A_KIND: {
            const auto& values = hand.values;
            if(values[0] == values[2]) return 3*values[0];
            if(values[1] == values[3]) return 3*values[1];
            if(values[2] == values[4]) return 3*values[2];
            return 0;
        }
        case Category::FOUR_OF_A_KIND: {
            const auto& values = hand.values;
            if(values[0] == values[3]) return 4*values[0];
            if(values[1] == values[4]) return 4*values[1];
            return 0;
        }
        case Category::FULL_HOUSE: {
            const auto& values = hand.values;
            if(values[0] == values[1] && values[2] == values[4]) return 25;
            if(values[0] == values[2] && values[3] == values[4]) return 25;
            return 0;
        }
        case Category::SMALL_STRAIGHT: {
            const auto& values = hand.values;
            if(values[1]+1 != values[2]) return 0;
            if(values[2]+1 != values[3]) return 0;
            if(values[0]+1 == values[1]) return 30;
            if(values[3]+1 == values[4]) return 30;
            return 0;
        }
        case Category::LARGE_STRAIGHT: {
            const auto& values = hand.values;
            if(values[0]+1 != values[1]) return 0;
            if(values[1]+1 != values[2]) return 0;
            if(values[2]+1 != values[3]) return 0;
            if(values[3]+1 != values[4]) return 0;
            return 40;
        }
        case Category::YATHSEE: {
            const auto& values = hand.values;
            if(values[0] == values[4]) return 50;
            return 0;
        }
        case Category::CHANCE: {
            return std::accumulate(hand.values.begin(), hand.values.end(), 0);
        }
    }
    assert(false);
    __builtin_unreachable();
}

PointEvaluator::eval_t PointEvaluator::eval(const Hand& hand) {
    eval_t ps;
    for(int cat = 0; cat < (int)Category::size; ++cat) {
        ps[cat] = eval(hand, (Category)cat);
    }
    return ps;
}


std::string PointEvaluator::toString(const eval_t& e) {
    std::string s;
    forAllCategories([&](Category c) {
        s += ::toString(c);
        s += " ";
        s += std::to_string(e[(int)c]);
        s += '\n';
    });
    return s;
}

std::unordered_map<HandId, PointEvaluator::EvaluationData> PointEvaluator::evaluationCache_ = []() {
    std::unordered_map<HandId, PointEvaluator::EvaluationData> cache;
    
    return cache;
}();

const PointEvaluator::EvaluationData& PointEvaluator::lookupEvaluation(const HandId& id) {
    auto it = evaluationCache_.find(id);
    if(it != evaluationCache_.end()) {
        return it->second;
    } else {
        auto e = eval(Hand::fromId(id));
        EvaluationData data { e };
        auto entry = evaluationCache_.emplace(std::make_pair(id, data));
        return entry.first->second;
    }
}
