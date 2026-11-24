#pragma once

#include "weighted_graph.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

struct LayeredBellmanFordResult {
    static constexpr long long unreachable = std::numeric_limits<long long>::max();

    // distance[k][v]: shortest distance to v using at most k edges.
    // Rows run from 0 through V; row V is used to detect negative cycles.
    std::vector<std::vector<long long>> distance;
    bool has_reachable_negative_cycle = false;
};

// Assumes intermediate path-weight sums fit in long long.
inline LayeredBellmanFordResult bellman_ford_layered(const WeightedGraph& graph, int source) {
    const std::size_t vertex_count = graph.size();
    if (source < 0 || static_cast<std::size_t>(source) >= vertex_count) {
        throw std::out_of_range("source is outside the graph");
    }

    LayeredBellmanFordResult result{
        std::vector<std::vector<long long>>(
            vertex_count + 1,
            std::vector<long long>(vertex_count, LayeredBellmanFordResult::unreachable)),
        false,
    };
    result.distance[0][source] = 0;
    auto& distance = result.distance;

    // TODO: For each k from 1 to V, first copy distance[k - 1] to distance[k].
    // Then iterate over every edge u -> v and relax distance[k][v] using
    // distance[k - 1][u]. Skip u when its previous-layer distance is unreachable.
    // TODO: Compare rows V - 1 and V to detect a reachable negative cycle.
    for (int k = 1; k <= vertex_count; ++k) {
        distance[k] = distance[k - 1];
        for (int u = 0; u < vertex_count; ++u) {
            if (distance[k - 1][u] == LayeredBellmanFordResult::unreachable)
                continue;
            for (auto neighbor : graph.neighbors(u)) {
                int v = neighbor.to;
                int weight = neighbor.weight;
                distance[k][v] = std::min(distance[k][v], distance[k - 1][u] + weight);
            }
        }        
    }
    for (int i = 0; i< vertex_count; ++i) {
        if (distance[vertex_count][i] < distance[vertex_count - 1][i]) {
            result.has_reachable_negative_cycle = true;
            break;
        }
    }
    return result;
}
