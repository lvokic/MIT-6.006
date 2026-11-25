#pragma once

#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Code {
    std::string filename;
    int duration;
};

struct Dependency {
    std::string before;
    std::string after;
};

// Return the earliest time all code completes, or nullopt for a dependency cycle.
// Filenames are unique, durations are positive, and dependency endpoints exist.
// For an empty job, return 0. Assume completion times fit in long long.
inline std::optional<long long> min_time(
    [[maybe_unused]] const std::vector<Code>& codes,
    [[maybe_unused]] const std::vector<Dependency>& dependencies) {

    std::unordered_map<std::string_view, int> duration;
    std::unordered_map<std::string_view, int> indegree;
    std::unordered_map<std::string_view, long long> earliest_finish;
    std::unordered_map<std::string_view, std::vector<std::string_view>> adjacency_;
    for (const auto& code : codes) {
        duration[code.filename] = code.duration;
        indegree[code.filename] = 0;
        earliest_finish[code.filename] = code.duration;
    }
    for (const auto& dependency : dependencies) {
        adjacency_[dependency.before].push_back(dependency.after);
        ++indegree[dependency.after];
    }

    int visited_cnt = 0;
    long long max_element = 0;
    std::queue<std::string_view> que;
    for (const auto& [view, in] : indegree) {
        if (in == 0) {
            que.push(view);
            earliest_finish[view] = duration[view];
        }
    }

    while (!que.empty()) {
        std::string_view view = que.front();
        visited_cnt++;
        que.pop();
        max_element = std::max(max_element, earliest_finish[view]);
        for (const auto& neighbor : adjacency_[view]) {
            indegree[neighbor]--;
            earliest_finish[neighbor] = std::max(earliest_finish[neighbor], 
                                                earliest_finish[view] + duration[neighbor]);
            if (indegree[neighbor] == 0)
                que.push(neighbor);
        }
    }
    if (visited_cnt < codes.size())
        return std::nullopt;
    
    return max_element;
}
