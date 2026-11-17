#pragma once

#include "graph.hpp"
#include <stdexcept>
#include <vector>

struct DFSResult {
    // parent[source] = source; unreachable vertices have parent = -1.
    std::vector<int> parent;

    // Each reachable vertex appears exactly once, in discovery order.
    std::vector<int> order;
};


inline void iterate(const Graph& graph, int source, int node,
                    std::vector<int>& parent, std::vector<int>& order) {
    if (parent[node] != -1)
        return;
    parent[node] = source;
    order.push_back(node);
    for (auto neighbor : graph.neighbors(node))
        iterate(graph, node, neighbor, parent, order);
}

// Search outgoing edges from source. Throw std::out_of_range for an invalid source.
// Expected complexity: O(V + E) time and O(V) auxiliary space.
inline DFSResult dfs(const Graph& graph, int source) {
    // TODO: Initialize parent and implement the DFS search.
    // You may use recursion or an explicit stack; record discovery order in order.
    std::size_t vertices = graph.size();

    std::vector<int> parent(vertices, -1);
    std::vector<int> order;
    order.reserve(vertices);

    iterate(graph, source, source, parent, order);
    return DFSResult{parent, order};
}
