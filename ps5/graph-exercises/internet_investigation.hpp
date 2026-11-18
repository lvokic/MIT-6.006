#pragma once

#include "graph.hpp"

#include <cstddef>
#include <queue>
#include <stdexcept>
#include <vector>

// Router IDs are 0, ..., router_count - 1. Each wire is bidirectional.
struct Wire {
    int from;
    int to;
    int length;  // Positive integer number of feet.
};

struct Edge {
    int from;
    int to;
};

// Replace each wire with a path of unit-length edges.
// Keep router IDs unchanged and number intermediate vertices from router_count.
// Add both directions of every unit edge using Graph::add_edge.
inline Graph convert([[maybe_unused]] std::size_t router_count,
                     [[maybe_unused]] const std::vector<Wire>& wires) {
    // TODO: Count intermediate vertices, create the graph, and add unit edges.
    std::vector<Edge> edges;
    for (const auto& wire : wires) {
        int previous = wire.from;
        for (int i = 1; i < wire.length; ++i) {
            int intermediate = static_cast<int>(router_count++);
            edges.push_back({previous, intermediate});
            previous = intermediate;
        }
        edges.push_back({previous, wire.to});
    }
    Graph graph(router_count);
    for (auto edge : edges) {
        graph.add_edge(edge.from, edge.to);
        graph.add_edge(edge.to, edge.from);
    }
    return graph;
}

// Return the distance from each vertex to its nearest source, or -1 if unreachable.
inline std::vector<int> multi_source_bfs([[maybe_unused]] const Graph& graph,
                                         [[maybe_unused]] const std::vector<int>& sources) {
    // TODO: Initialize all sources at distance 0, then perform BFS.
    std::queue<int> que;
    std::vector<int> distances(graph.size(), -1);
    for (auto entry : sources) {
        que.push(entry);
        distances[entry] = 0;
    }
    while (!que.empty()) {
        int entry = que.front();
        que.pop();
        for (auto neighbor : graph.neighbors(entry)) {
            if (distances[neighbor] != -1) continue;
            distances[neighbor] = distances[entry] + 1;
            que.push(neighbor);
        }
    }
    return distances;
}

// Assumes valid router IDs, positive wire lengths, and at least one reachable
// entry point for every router, as in the problem statement.
inline long long total_latency(std::size_t router_count,
                               const std::vector<Wire>& wires,
                               const std::vector<int>& entry_points) {
    Graph graph = convert(router_count, wires);
    std::vector<int> distance = multi_source_bfs(graph, entry_points);

    long long total = 0;
    for (std::size_t router = 0; router < router_count; ++router) {
        total += distance[router];
    }
    return total;
}
