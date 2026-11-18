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

inline void dfs([[maybe_unused]] const WeightedGraph& graph, std::vector<int>& res, std::vector<bool>& visited, int source) {
    if (visited[source]) return;
    visited[source] = true;
    for (auto neighbor : graph.neighbors(source)) {
        if (visited[neighbor.to]) continue;
        dfs(graph, res, visited, neighbor.to);
    }
    res.push_back(source);
}

// Return all vertices in an order that puts u before v for every edge u -> v.
// The input graph is assumed to be a DAG.
inline std::vector<int> topological_order([[maybe_unused]] const WeightedGraph& graph) {
    // TODO: Compute the topological order.
    std::size_t vetices = graph.size();
    std::vector<bool> visited(vetices, false);
    std::vector<int> order; order.reserve(vetices);
    for (std::size_t i = 0; i < vetices; ++i)
        dfs(graph, order, visited, i);
    std::reverse(order.begin(), order.end());
    return order;
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

    size_t index = 0;
    std::vector<int> order = topological_order(graph);
    for (; index < order.size(); ++index)
        if (order[index] == source) break;
    
    if (index > order.size()) return result;
    while (index < order.size()) {
        int entry = order[index];
        int weight; int node;
        const std::vector<WeightedEdge>& neighbors = graph.neighbors(entry);
        if (neighbors.empty()) {
            index++; continue;
        }
        for (auto neighbor : neighbors) {
            weight = neighbor.weight;
            node = neighbor.to;
            if (result.distance[node] > weight + result.distance[entry]) {
                result.distance[node] = weight + result.distance[entry];
                result.parent[node] = entry;
            }
        }
        index++;
    }

    // TODO: Visit vertices in topological order and relax outgoing edges.
    // Skip unreachable vertices. Update the parent when a distance improves.
    return result;
}
