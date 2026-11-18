#pragma once

#include "weighted_graph.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

struct DAGResult {
    static constexpr long long unreachable = std::numeric_limits<long long>::max();

    // parent[source] = source; unreachable vertices have parent = -1.
    std::vector<int> parent;
    std::vector<long long> distance;
};

// Return all vertices in an order that puts u before v for every edge u -> v.
// The input graph is assumed to be a DAG.
inline std::vector<int> topological_order([[maybe_unused]] const WeightedGraph& graph) {
    // TODO: Compute the topological order.
    throw std::logic_error("TODO: implement topological_order");
}

// Negative edge weights are allowed. Target: O(V + E) time and O(V) auxiliary space.
inline DAGResult dag_shortest_paths(const WeightedGraph& graph, int source) {
    if (source < 0 || static_cast<std::size_t>(source) >= graph.size()) {
        throw std::out_of_range("source is outside the graph");
    }

    DAGResult result{
        std::vector<int>(graph.size(), -1),
        std::vector<long long>(graph.size(), DAGResult::unreachable),
    };
    result.parent[source] = source;
    result.distance[source] = 0;

    // TODO: Visit vertices in topological order and relax outgoing edges.
    // Skip unreachable vertices. Update the parent when a distance improves.
    throw std::logic_error("TODO: implement DAG relaxation");
}
