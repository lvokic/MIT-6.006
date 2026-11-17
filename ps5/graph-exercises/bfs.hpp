#pragma once

#include "graph.hpp"
#include <stdexcept>
#include <vector>
#include <queue>

struct BFSResult {
    // parent[source] = source; unreachable vertices have parent = -1.
    std::vector<int> parent;

    // Shortest number of edges from source; unreachable vertices have distance = -1.
    std::vector<int> distance;
};

inline void iterate(const Graph& graph, int source, 
                    std::vector<int>& parent, std::vector<int>& distance, 
                    std::vector<bool>& visited) {
    std::queue<int> que;
    que.push(source);
    while (!que.empty()) {
        int source = que.front();
        que.pop();
        for (auto neighbor : graph.neighbors(source)) {
            if (visited[neighbor]) continue;
            visited[neighbor] = true;
            parent[neighbor] = source;
            distance[neighbor] = distance[source] + 1;
            que.push(neighbor);
        }
    }
}

// Search outgoing edges from source. Throw std::out_of_range for an invalid source.
// Expected complexity: O(V + E) time and O(V) auxiliary space.
inline BFSResult bfs(const Graph& graph, int source) {
    // TODO: Initialize parent and distance, then use a queue to discover vertices.
    // Record the parent and distance when a vertex is first discovered.
    std::size_t vertices = graph.size();

    std::vector<int> parent(vertices, -1);
    std::vector<int> distance(vertices, -1);
    std::vector<bool> visited(vertices, false);
    
    parent[source] = source;
    distance[source] = 0;
    visited[source] = true;
    iterate(graph, source, parent, distance, visited);

    return BFSResult{parent, distance};
}
