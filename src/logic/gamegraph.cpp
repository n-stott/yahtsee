#include "logic/gamegraph.h"
#include "logic/evaluator.h"
#include "logic/simulator.h"

GameGraph::map<HandId, GameGraph::map<RethrowId, GameGraph::MultiEdge>> GameGraph::graph_;
GameGraph::map<HandId, PointEvaluator::eval_t> GameGraph::handEval_;
GameGraph::map<HandId, GameGraph::map<RethrowId, PointEvaluator::eval_t>> GameGraph::expectedScore1_;
GameGraph::map<HandId, GameGraph::map<RethrowId, PointEvaluator::eval_t>> GameGraph::expectedScore2_;

void GameGraph::create() {
    Simulator::forAllThrows(5, [](const Throw& t) {
        Hand hand = Hand::fromArray(t.values);
        graph_[hand.toId()] = {};
    });

    std::cout << graph_.size() << std::endl;

    for(auto it = graph_.begin(); it != graph_.end(); ++it) {
        HandId hid = it->first;
        Hand hand = Hand::fromId(hid);
        auto& edges = it->second;
        Simulator::forAllRethrows2(hand, [&](const Hand& hand, const Rethrow& rt) {
            auto& destinations = edges[rt.toId()];
            Simulator::forAllThrows(5-rt.size, [&](const Throw& t) {
                Hand h = combine(rt, t);
                destinations.emplace_back(std::make_pair(h.toId(), 1));
            });
            if(destinations.empty()) return;
            std::sort(destinations.begin(), destinations.end(), [](const auto& a, const auto& b) {
                return a.first.id < b.first.id;
            });
            MultiEdge filteredDestinations;
            filteredDestinations.emplace_back(destinations[0]);
            for(size_t i = 1; i < destinations.size(); ++i) {
                if(destinations[i].first.id == filteredDestinations.back().first.id) {
                    ++filteredDestinations.back().second;
                } else {
                    filteredDestinations.emplace_back(destinations[i]);
                }
            }
            std::swap(destinations, filteredDestinations);
        });
    }

    for(auto it = graph_.begin(); it != graph_.end(); ++it) {
        handEval_[it->first] = PointEvaluator::eval(Hand::fromId(it->first));
    }

    for(auto it = graph_.begin(); it != graph_.end(); ++it) {
        auto& es = expectedScore1_[it->first];
        for(auto rit = it->second.begin(); rit != it->second.end(); ++rit) {
            es[rit->first] = Simulator::expectedScore(Hand::fromId(it->first), Rethrow::fromId(rit->first));
        }
    }

    // for(auto it = graph_.begin(); it != graph_.end(); ++it) {
    //     auto& es = expectedScore2_[it->first];
    //     for(auto rit = it->second.begin(); rit != it->second.end(); ++rit) {
    //         es[rit->first] = Simulator::expectedScore2(Hand::fromId(it->first), Rethrow::fromId(rit->first));
    //     }
    // }
}