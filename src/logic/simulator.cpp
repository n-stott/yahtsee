#include "logic/simulator.h"
#include <unordered_set>

std::unordered_map<HandId, std::vector<Rethrow>> Simulator::rethrows_ = []() -> std::unordered_map<HandId, std::vector<Rethrow>> {
    std::unordered_map<HandId, std::vector<Rethrow>> rethrows;

    Simulator::forAllThrows(5, [&](const Throw& t) {
        Hand hand = Hand::fromArray(t.values);
        HandId hid = hand.toId();
        if(rethrows.contains(hid)) return;
        auto& rts = rethrows[hid];
        assert(rts.empty());
        std::unordered_set<RethrowId> rtids;
        forAllRethrows(hand, [&](const Hand& hand, const Rethrow& rt) {
            rtids.insert(rt.toId());
        });
        for(auto rtid : rtids) rts.push_back(Rethrow::fromId(rtid));
    });
    return rethrows;
}();