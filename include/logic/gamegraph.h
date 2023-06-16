#ifndef GAMEGRAPH_H
#define GAMEGRAPH_H

#include "logic/round.h"
#include <unordered_map>
#include <vector>

struct GameGraph {
    template<typename K, typename V>
    using map = std::unordered_map<K, V>;

    using MultiEdge = std::vector<std::pair<HandId, int>>;

    static map<HandId, map<RethrowId, MultiEdge>> graph_;

    static void create();
};

#endif