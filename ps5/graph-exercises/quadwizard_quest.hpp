#pragma once

#include "graph.hpp"

#include <cstddef>
#include <queue>
#include <stdexcept>
#include <vector>

struct Door {
    int room1;
    int room2;
    bool enchanted;
};

inline void bfs(int source, const Graph& graph, std::vector<bool>& visited) {
    std::queue<int> que;
    que.push(source);
    visited[source] = true;
    while (!que.empty()) {
        int entry = que.front();
        que.pop();
        for (auto neighbor : graph.neighbors(entry)) {
            if (visited[neighbor] == true) continue;
            que.push(neighbor);
            visited[neighbor] = true;
        }
    }
}

// Rooms are numbered 0, ..., room_count - 1. Doors are bidirectional.
// The labyrinth is assumed to be connected and contain at least one room.
inline int minimum_disenchanted_doors(std::size_t room_count,
                                       const std::vector<Door>& doors) {
    Graph ordinary_doors(room_count);
    for (const Door& door : doors) {
        if (!door.enchanted) {
            ordinary_doors.add_edge(door.room1, door.room2);
            ordinary_doors.add_edge(door.room2, door.room1);
        }
    }

    int groups = 0;
    std::vector<bool> visited(room_count, false);
    for (int i = 0; i < room_count; ++i) {
        if (visited[i])
            continue;
        bfs(i, ordinary_doors, visited);
        groups++;
    }

    // TODO: Use BFS or DFS to count components of ordinary_doors.
    // Return the number of components minus one.
    return groups - 1;
}
