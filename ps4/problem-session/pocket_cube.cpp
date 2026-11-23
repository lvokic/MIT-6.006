#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using Config = std::string;
using Move = std::pair<int, int>; // face, direction (+1 clockwise, -1 counterclockwise)
using ParentMap = std::unordered_map<Config, std::optional<Config>>;

const Config SOLVED = "000011223344112233445555";

std::string config_str(const Config& config) {
    assert(config.size() == 24);
    return "  " + config.substr(0, 2) + "\n" +
           "  " + config.substr(2, 2) + "\n" +
           config.substr(4, 8) + "\n" +
           config.substr(12, 8) + "\n" +
           "  " + config.substr(20, 2) + "\n" +
           "  " + config.substr(22, 2) + "\n";
}

template <std::size_t N>
void shift(Config& config, int distance, const std::array<int, N>& positions) {
    std::array<char, N> values{};
    for (std::size_t i = 0; i < N; ++i) {
        values[i] = config[positions[i]];
    }
    const int count = static_cast<int>(N);
    for (int i = 0; i < count; ++i) {
        int source = ((i - distance) % count + count) % count;
        config[positions[i]] = values[source];
    }
}

Config rotate(const Config& config, int face, int direction) {
    assert(config.size() == 24);
    assert(face >= 0 && face <= 2);
    assert(direction == -1 || direction == 1);
    Config result = config;
    if (face == 0) {
        shift(result, direction, std::array<int, 4>{0, 1, 3, 2});
        shift(result, 2 * direction, std::array<int, 8>{11, 10, 9, 8, 7, 6, 5, 4});
    } else if (face == 1) {
        shift(result, direction, std::array<int, 4>{4, 5, 13, 12});
        shift(result, 2 * direction, std::array<int, 8>{0, 2, 6, 14, 20, 22, 19, 11});
    } else {
        shift(result, direction, std::array<int, 4>{6, 7, 15, 14});
        shift(result, 2 * direction, std::array<int, 8>{2, 3, 8, 16, 21, 20, 13, 5});
    }
    return result;
}

std::vector<Config> neighbors(const Config& config) {
    std::vector<Config> result;
    result.reserve(6);
    for (int face = 0; face < 3; ++face) {
        for (int direction : {-1, 1}) {
            result.push_back(rotate(config, face, direction));
        }
    }
    return result;
}

std::vector<Config> explore_frontier(const std::vector<Config>& frontier,
                                     ParentMap& parent, bool verbose = false) {
    if (verbose) {
        std::cout << "Exploring next frontier containing # configs: " << frontier.size() << '\n';
    }
    std::vector<Config> next;
    for (const Config& current : frontier) {
        for (const Config& neighbor : neighbors(current)) {
            if (parent.emplace(neighbor, current).second) {
                next.push_back(neighbor);
            }
        }
    }
    return next;
}

std::optional<Config> check_duplicate(const std::vector<Config>& frontier, const ParentMap& other_parent) {
    for (const Config& config : frontier) {
        if (other_parent.count(config)) {
            return config;
        }
    }
    return std::nullopt;
}

std::vector<Config> path_to_config(const Config& config, const ParentMap& parent) {
    std::vector<Config> path;
    std::optional<Config> current = config;
    while (current) {
        path.push_back(*current);
        current = parent.at(*current);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Config> path_to_solved(const Config& config, const ParentMap& parent) {
    std::vector<Config> path;
    std::optional<Config> current = config;
    while (current) {
        path.push_back(*current);
        current = parent.at(*current);
    }
    path.erase(path.begin());
    return path;
}

std::optional<std::vector<Move>> moves_from_path(const std::vector<Config>& path) {
    std::vector<Move> moves;
    for (std::size_t i = 1; i < path.size(); ++i) {
        bool found = false;
        for (int face = 0; face < 3 && !found; ++face) {
            for (int direction : {-1, 1}) {
                if (rotate(path[i - 1], face, direction) == path[i]) {
                    moves.emplace_back(face, direction);
                    found = true;
                    break;
                }
            }
        }
        if (!found) return std::nullopt;
    }
    return moves;
}

std::vector<Config> path_from_moves(Config config, const std::vector<Move>& moves) {
    std::vector<Config> path{config};
    for (const auto& [face, direction] : moves) {
        config = rotate(config, face, direction);
        path.push_back(config);
    }
    return path;
}

Config scramble(Config config, int n) {
    std::mt19937 generator(std::random_device{}());
    // The supplied Python template selects neighbors 0 through 2.
    std::uniform_int_distribution<int> distribution(0, 2);
    for (int i = 0; i < n; ++i) {
        config = neighbors(config)[distribution(generator)];
    }
    return config;
}

bool check(const Config& config, const std::vector<Move>& moves, bool verbose = false) {
    std::vector<Config> path = path_from_moves(config, moves);
    if (verbose) {
        std::cout << "Making " << moves.size() << " moves from starting configuration:\n";
        std::cout << config_str(config);
        for (std::size_t i = 0; i < moves.size(); ++i) {
            const auto& [face, direction] = moves[i];
            std::cout << "Rotating face " << face << ' '
                      << (direction == 1 ? "clockwise" : "counterclockwise") << ":\n";
            std::cout << config_str(path[i + 1]);
        }
    }
    return path.back() == SOLVED;
}

// Search from both endpoints, expanding one complete BFS layer at a time.
std::optional<std::vector<Move>> solve(const Config& config) {
    if (config == SOLVED)
        return std::vector<Move>{};

    ParentMap parent_config{{config, std::nullopt}};
    ParentMap parent_solved{{SOLVED, std::nullopt}};
    std::vector<Config> frontier_config{config};
    std::vector<Config> frontier_solved{SOLVED};

    std::optional<Config> result{std::nullopt};
    while (!frontier_config.empty() && !frontier_solved.empty()) {
        frontier_config = explore_frontier(frontier_config, parent_config, false);
        result = check_duplicate(frontier_config, parent_solved);
        if (result.has_value()) {
            std::vector<Config> path_config = path_to_config(result.value(), parent_config);
            std::vector<Config> path_solved = path_to_solved(result.value(), parent_solved);
            path_config.insert(path_config.end(), path_solved.begin(), path_solved.end());
            return moves_from_path(path_config);
        }
        frontier_solved = explore_frontier(frontier_solved, parent_solved, false);
        result = check_duplicate(frontier_solved, parent_config);
        if (result.has_value()) {
            std::vector<Config> path_config = path_to_config(result.value(), parent_config);
            std::vector<Config> path_solved = path_to_solved(result.value(), parent_solved);
            path_config.insert(path_config.end(), path_solved.begin(), path_solved.end());
            return moves_from_path(path_config);
        }
    }
    return std::nullopt;
}

void test(const Config& config) {
    std::cout << "Solving configuration:\n" << config_str(config);
    auto moves = solve(config);
    if (!moves) {
        std::cout << "Path to solved state not found.\n";
        return;
    }
    std::cout << "Path to solved state found!\n";
    std::cout << (check(config, *moves) ? "Move sequence terminated at solved state!\n"
                                        : "Move sequence did not terminate at solved state.\n");
}

#ifndef POCKET_CUBE_NO_MAIN
int main() {
    test(scramble(SOLVED, 100));
}
#endif
