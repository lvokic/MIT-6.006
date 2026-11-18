#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

struct WeightedEdge {
    int to;
    int weight;
};

// Directed graph with vertices numbered 0, 1, ..., vertex_count - 1.
class WeightedGraph {
public:
    explicit WeightedGraph(std::size_t vertex_count) : adjacency_(vertex_count) {}

    std::size_t size() const noexcept { return adjacency_.size(); }

    void add_edge(int from, int to, int weight) {
        check_vertex(from);
        check_vertex(to);
        adjacency_[static_cast<std::size_t>(from)].push_back({to, weight});
    }

    const std::vector<WeightedEdge>& neighbors(int vertex) const {
        check_vertex(vertex);
        return adjacency_[static_cast<std::size_t>(vertex)];
    }

private:
    void check_vertex(int vertex) const {
        if (vertex < 0 || static_cast<std::size_t>(vertex) >= size()) {
            throw std::out_of_range("vertex is outside the graph");
        }
    }

    std::vector<std::vector<WeightedEdge>> adjacency_;
};
