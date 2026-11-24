#pragma once

#include "weighted_graph.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

struct BellmanFordResult {
    static constexpr long long unreachable = std::numeric_limits<long long>::max();

    // parent[source] = source; unreachable vertices have parent = -1.
    std::vector<int> parent;
    std::vector<long long> distance;

    // True if any negative-weight cycle is reachable from source.
    // When true, distance and parent are not guaranteed to be shortest paths.
    bool has_reachable_negative_cycle = false;
};

struct BellmanFordEdge {
    int from;
    int to;
    int weight;
};

// O(V + VE) time and O(V + E) auxiliary space for this edge-list version.
// Assumes intermediate path-weight sums fit in long long.
inline BellmanFordResult bellman_ford(const WeightedGraph& graph, int source) {
    if (source < 0 || static_cast<std::size_t>(source) >= graph.size()) {
        throw std::out_of_range("source is outside the graph");
    }

    BellmanFordResult result{
        std::vector<int>(graph.size(), -1),
        std::vector<long long>(graph.size(), BellmanFordResult::unreachable),
        false,
    };
    result.parent[source] = source;
    result.distance[source] = 0;
    auto& parent = result.parent;
    auto& distance = result.distance;
    int vertex_count = graph.size();

    // Flatten the adjacency list so each relaxation pass scans only the edges.
    std::vector<BellmanFordEdge> edges;
    for (std::size_t from = 0; from < graph.size(); ++from) {
        for (const WeightedEdge& edge : graph.neighbors(static_cast<int>(from))) {
            edges.push_back({static_cast<int>(from), edge.to, edge.weight});
        }
    }

    // TODO: Relax every edge up to V - 1 times, updating distance and parent.
    // Skip edges whose from vertex is unreachable, before adding its weight.
    // Optionally stop early if an entire pass makes no update.
    // TODO: Scan the edges once more to detect a reachable negative cycle.
    for (int k = 1; k < vertex_count; ++k) {
        for (const auto& edge : edges) {
            int from = edge.from;
            int to = edge.to;
            int weight = edge.weight;
            if (distance[from] == BellmanFordResult::unreachable)
                continue;
            if (distance[from] + weight < distance[to]) {
                distance[to] = distance[from] + weight;
                parent[to] = from;
            }
        }
    }
    for (const auto& edge : edges) {
        int from = edge.from;
        int to = edge.to;
        int weight = edge.weight;
        if (distance[from] == BellmanFordResult::unreachable)
            continue;
        if (distance[from] + weight < distance[to]) {
            result.has_reachable_negative_cycle = true;
            break;
        }
    }
    return result;
}
