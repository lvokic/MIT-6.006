#pragma once

#include "../graph-exercises/weighted_graph.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

struct Landmark {
    long long x;
    long long y;
    bool inside_honor_stone;
};

// A trail can be traversed in either direction and has a positive length.
// Landmark IDs are indices into the landmarks vector.
struct Trail {
    int first;
    int second;
    int length;
};

struct NeverReturnRoute {
    long long length;
    // Landmark IDs in order, including the gorge and the outside destination.
    std::vector<int> path;
};

struct NeverReturnDAGResult {
    static constexpr long long unreachable = std::numeric_limits<long long>::max();

    // parent[source] = source; unreachable vertices have parent = -1.
    std::vector<int> parent;
    std::vector<long long> distance;
};


inline long long distanceToGorge(const std::vector<Landmark>& landmarks, int node, int gorge) {
    long long delta_x = landmarks[node].x - landmarks[gorge].x;
    long long delta_y = landmarks[node].y - landmarks[gorge].y;
    return delta_x * delta_x + delta_y * delta_y;
}

inline void DFS(const WeightedGraph& graph, std::vector<int>& order,
                std::vector<bool>& visited, int source) {
    visited[source] = true;
    for (const auto& neighbor : graph.neighbors(source)) {
        if (visited[neighbor.to] == true)
            continue;
        DFS(graph, order, visited, neighbor.to);
    }
    order.push_back(source);
}

inline std::vector<int> topologicalOrder(const WeightedGraph& graph) {
    int vertex_count = graph.size();
    std::vector<int> order;
    order.reserve(vertex_count);
    std::vector<bool> visited(vertex_count, false);
    for (int i = 0; i < vertex_count; ++i) {
        if (visited[i] == true) continue;
        DFS(graph, order, visited, i);
    }
    std::reverse(order.begin(), order.end());
    return order;
}

// Input consists of the n inside landmarks and their outside neighbors, plus
// the trails needed to reach an outside neighbor. The gorge is inside.
// Assume IDs are valid and squared coordinate differences fit in long long.
// The input size is O(n) because each inside landmark has at most five trails.
// Return nullopt if no outside landmark is reachable without returning.
inline std::optional<NeverReturnRoute> shortest_never_return_route(
    const std::vector<Landmark>& landmarks,
    const std::vector<Trail>& trails,
    int gorge) {
    if (gorge < 0 || static_cast<std::size_t>(gorge) >= landmarks.size()) {
        throw std::out_of_range("gorge is outside the landmarks vector");
    }
    int vertex_count = landmarks.size();
    WeightedGraph graph(vertex_count);
    for (const auto& trail : trails) {
        long long first_distance = distanceToGorge(landmarks, trail.first, gorge);
        long long second_distance = distanceToGorge(landmarks, trail.second, gorge);
        if (first_distance < second_distance)
            graph.add_edge(trail.first, trail.second, trail.length);
        else if (second_distance < first_distance)
            graph.add_edge(trail.second, trail.first, trail.length);
    }

    NeverReturnDAGResult result{
        std::vector<int>(graph.size(), -1),
        std::vector<long long>(graph.size(), NeverReturnDAGResult::unreachable),
    };
    result.parent[gorge] = gorge;
    result.distance[gorge] = 0;

    std::vector<int> topological_order = topologicalOrder(graph);
    std::size_t index = 0;
    while (index < topological_order.size()) {
        if (topological_order[index] == gorge)
            break;
        index++;
    }
    if (index == topological_order.size()) return std::nullopt;
    while (index < topological_order.size()) {
        int entry = topological_order[index];
        int weight, node;
        for (const auto& neighbor : graph.neighbors(entry)) {
            if (result.distance[entry] == NeverReturnDAGResult::unreachable)
                continue;
            weight = neighbor.weight;
            node = neighbor.to;
            if (result.distance[node] > weight + result.distance[entry]) {
                result.distance[node] = weight + result.distance[entry];
                result.parent[node] = entry;
            }
        }
        index++;
    }
    int destination = -1;
    for (int i = 0; i < vertex_count; ++i) {
        if (result.distance[i] == NeverReturnDAGResult::unreachable || landmarks[i].inside_honor_stone)
            continue;
        if (destination == -1 || result.distance[i] < result.distance[destination])
            destination = i;
    }
    if (destination == -1) return std::nullopt;
    std::vector<int> path;
    int node = destination;
    while (node != gorge) {
        path.push_back(node);
        node = result.parent[node];
    }
    path.push_back(gorge);
    std::reverse(path.begin(), path.end());
    return NeverReturnRoute{result.distance[destination], path};
}
