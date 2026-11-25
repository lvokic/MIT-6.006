#pragma once

#include "../graph-exercises/bellman_ford.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

struct Conversion {
    std::uint64_t input_count;   // Positive power of two.
    int input_type;              // Type IDs are 1, ..., type_count.
    std::uint64_t output_count;  // Positive power of two.
    int output_type;
};

inline bool dig_bellman_ford(const WeightedGraph& graph, int source) {
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

    for (int k = 1; k < vertex_count; ++k) {
        for (int u = 0; u < vertex_count; ++u) {
            if (distance[u] == BellmanFordResult::unreachable)
                continue;
            for (const auto& edge : graph.neighbors(u)) {
                int to = edge.to;
                int weight = edge.weight;
                if (distance[u] + weight < distance[to]) {
                    distance[to] = distance[u] + weight;
                    parent[to] = u;
                }
            }
        }
    }

    for (int u = 0; u < vertex_count; ++u) {
        if (distance[u] == BellmanFordResult::unreachable)
            continue;
        for (const auto& edge : graph.neighbors(u)) {
            int to = edge.to;
            int weight = edge.weight;
            if (distance[u] + weight < distance[to]) {
                return true;
            }
        }
    }
    return false;
}

inline int conversion_weight(std::uint64_t input_count,
                             std::uint64_t output_count) {
    int input_exponent = std::countr_zero(input_count);
    int output_exponent = std::countr_zero(output_count);
    return input_exponent - output_exponent;
}

// Return true if the rules allow an unbounded increase in block count.
// Assumes valid type IDs and counts representable by uint64_t.
inline bool should_disallow_conversions(
    std::size_t type_count,
    const std::vector<Conversion>& conversions) {
    // Build a directed WeightedGraph with vertices 0, ..., type_count.
    // Use 0 as a super-source and add a zero-weight edge to every block type.
    // For each rule, add input_type -> output_type with weight
    // log2(input_count) - log2(output_count).
    // Run Bellman-Ford from the super-source to detect a negative cycle.
    WeightedGraph graph(type_count + 1);
    for (const auto convert : conversions) {
        int from = convert.input_type;
        int to = convert.output_type;
        int weight = conversion_weight(convert.input_count, convert.output_count);
        graph.add_edge(from, to, weight);
    }

    int source = 0;
    for (std::size_t i = 1; i <= type_count; ++i)
        graph.add_edge(source, static_cast<int>(i), 0);
    return dig_bellman_ford(graph, source);
}
