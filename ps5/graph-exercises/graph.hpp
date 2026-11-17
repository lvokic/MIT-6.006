#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

// Vertices are numbered 0, 1, ..., vertex_count - 1.
class Graph {
public:
    explicit Graph(std::size_t vertex_count) : adjacency_(vertex_count) {}

    std::size_t size() const noexcept { return adjacency_.size(); }

    // Adds a directed edge. For an undirected graph, add both directions.
    void add_edge(int from, int to) {
        check_vertex(from);
        check_vertex(to);
        adjacency_[static_cast<std::size_t>(from)].push_back(to);
    }

    const std::vector<int>& neighbors(int vertex) const {
        check_vertex(vertex);
        return adjacency_[static_cast<std::size_t>(vertex)];
    }

private:
    void check_vertex(int vertex) const {
        if (vertex < 0 || static_cast<std::size_t>(vertex) >= size()) {
            throw std::out_of_range("vertex is outside the graph");
        }
    }

    std::vector<std::vector<int>> adjacency_;
};
