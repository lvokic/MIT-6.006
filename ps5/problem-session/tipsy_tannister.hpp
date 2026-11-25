#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <queue>
#include <stdexcept>
#include <vector>

// Town IDs are 0, ..., town_count - 1; roads can be traveled both ways.
struct TipsyRoad {
    int first;
    int second;
    long long tax;
};

// A state records the current town and the number of roads traveled modulo 3.
struct TipsyEdge {
    int from;
    int to;
    long long gain;
};

struct TipsyGraph {
    std::size_t state_count;
    std::vector<TipsyEdge> edges;
};

enum class TipsyStatus { unreachable, finite, unbounded };

static constexpr long long unreachable = std::numeric_limits<long long>::min();

struct TipsyResult {
    TipsyStatus status;
    long long gold = 0;  // Meaningful only when status is finite.
};

// Encode (town, roads traveled modulo 3) as 3 * town + remainder.
// Start at (source, 0). Each road increments the remainder; arriving with
// remainder 0 incurs the destination town's drinking cost.
// Do not add outgoing edges from destination states: the trip ends on arrival.
inline TipsyGraph build_tipsy_graph(
    [[maybe_unused]] const std::vector<long long>& drink_cost,
    [[maybe_unused]] const std::vector<TipsyRoad>& roads,
    [[maybe_unused]] int destination) {
    const std::size_t town_count = drink_cost.size();
    TipsyGraph graph{
        town_count * 3,
        {}
    };
    graph.edges.reserve(roads.size() * 6);
    auto add_direction = [&](int from, int to, long long tax) {
        if (from == destination) return;
        for (int remainder = 0; remainder < 3; ++remainder) {
            int next_remainder = (remainder + 1) % 3;
            int from_state = from * 3 + remainder;
            int to_state = to * 3 + next_remainder;
            long long gain = tax;
            if (next_remainder == 0)
                gain -= drink_cost[to];
            graph.edges.push_back({from_state, to_state, gain});
        }
    };
    for (const TipsyRoad& road : roads) {
        add_direction(road.first, road.second, road.tax);
        add_direction(road.second, road.first, road.tax);
    }
    return graph;
}

// Mark states from which any destination state can be reached, using a
// reverse graph and DFS or BFS.
inline std::vector<bool> can_reach_destination(
    const TipsyGraph& graph,
    int destination) {
    std::vector<std::vector<int>> reverse_adjacency(graph.state_count);
    for (const TipsyEdge& edge : graph.edges)
        reverse_adjacency[edge.to].push_back(edge.from);

    std::vector<bool> res(graph.state_count, false);
    std::queue<int> pending;
    for (int remainder = 0; remainder < 3; ++remainder) {
        int state = destination * 3 + remainder;
        res[state] = true;
        pending.push(state);
    }

    while (!pending.empty()) {
        int state = pending.front();
        pending.pop();
        for (int previous : reverse_adjacency[state]) {
            if (res[previous])
                continue;
            res[previous] = true;
            pending.push(previous);
        }
    }
    return res;
}

// Return the greatest attainable gain, unbounded if a reachable positive-gain
// cycle can also reach the destination, or unreachable if no route exists.
// Assume all intermediate gains fit in long long and exceed unreachable.
inline TipsyResult max_tipsy_gold(
    [[maybe_unused]] const std::vector<long long>& drink_cost,
    [[maybe_unused]] const std::vector<TipsyRoad>& roads,
    [[maybe_unused]] int source,
    [[maybe_unused]] int destination) {
    TipsyGraph graph = build_tipsy_graph(drink_cost, roads, destination);
    std::vector<bool> can_reach = can_reach_destination(graph, destination);

    TipsyResult result;
    long long max_element = unreachable;
    source = source * 3 + 0;
    if (can_reach[source] == false)
        return {TipsyStatus::unreachable};
    std::vector<long long> golds(graph.state_count, unreachable);
    golds[source] = 0;
    for (std::size_t k = 1; k < graph.state_count; ++k) {
        for (const auto& edge : graph.edges) {
            int from = edge.from;
            int to   = edge.to;
            long long gain = edge.gain;
            if (golds[from] == unreachable)
                continue;
            long long candidate = golds[from] + gain;
            golds[to] = std::max(golds[to], candidate);
        }
    }
    for (const auto& edge : graph.edges) {
        int from = edge.from;
        int to   = edge.to;
        long long gain = edge.gain;
        if (golds[from] == unreachable)
            continue;
        if (golds[to] < golds[from] + gain && can_reach[to]) {
            return {TipsyStatus::unbounded};
        }
    }
    for (int remainder = 0; remainder < 3; ++remainder) {
        int end_state = destination * 3 + remainder;
        max_element = std::max(max_element, golds[end_state]);
    }
    result.gold = max_element;
    result.status = TipsyStatus::finite;
    return result;
}
