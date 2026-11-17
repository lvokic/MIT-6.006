#include "bfs.hpp"
#include "dfs.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

bool has_edge(const Graph& graph, int from, int to) {
    const auto& neighbors = graph.neighbors(from);
    return std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
}

int main() {
    // Includes a cycle, multiple paths to vertex 3, and disconnected vertex 6.
    Graph graph(7);
    for (const auto& edge : std::vector<std::pair<int, int>>{
             {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}, {4, 1}, {4, 5}}) {
        graph.add_edge(edge.first, edge.second);
    }

    bool missing = false;
    try {
        BFSResult result = bfs(graph, 0);
        assert(result.parent.size() == graph.size());
        assert((result.distance == std::vector<int>{0, 1, 1, 2, 3, 4, -1}));
        assert(result.parent[0] == 0 && result.parent[6] == -1);
        for (int v = 1; v < 6; ++v) {
            int parent = result.parent[v];
            assert(parent >= 0 && parent < 6);
            assert(has_edge(graph, parent, v));
            assert(result.distance[parent] + 1 == result.distance[v]);
        }
        bool invalid_source = false;
        try {
            bfs(graph, -1);
        } catch (const std::out_of_range&) {
            invalid_source = true;
        }
        assert(invalid_source);
        std::cout << "BFS checks passed\n";
    } catch (const std::logic_error& error) {
        std::cout << "BFS incomplete: " << error.what() << '\n';
        missing = true;
    }

    try {
        DFSResult result = dfs(graph, 0);
        assert(result.parent.size() == graph.size());
        assert(result.parent[0] == 0 && result.parent[6] == -1);
        assert(result.order.size() == 6 && result.order.front() == 0);
        std::vector<int> discovery_index(graph.size(), -1);
        for (std::size_t i = 0; i < result.order.size(); ++i) {
            int v = result.order[i];
            assert(v >= 0 && v < 6 && discovery_index[v] == -1);
            discovery_index[v] = static_cast<int>(i);
        }
        for (int v = 1; v < 6; ++v) {
            int parent = result.parent[v];
            assert(parent >= 0 && parent < 6);
            assert(has_edge(graph, parent, v));
            assert(discovery_index[parent] < discovery_index[v]);
        }
        bool invalid_source = false;
        try {
            dfs(graph, 7);
        } catch (const std::out_of_range&) {
            invalid_source = true;
        }
        assert(invalid_source);
        std::cout << "DFS checks passed\n";
    } catch (const std::logic_error& error) {
        std::cout << "DFS incomplete: " << error.what() << '\n';
        missing = true;
    }

    return missing ? 1 : 0;
}
