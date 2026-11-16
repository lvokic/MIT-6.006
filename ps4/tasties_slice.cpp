#include "AVL_tree.hpp"
#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

class TasteSlice {
private:
    std::vector<Taste> tastes_;

public:
    explicit TasteSlice(std::vector<Taste> tastes) : tastes_(std::move(tastes)) {}

    // Return the best (x, y, taste), or nullopt when there are no toppings.
    std::optional<Taste> tastest_slice() {
        std::sort(tastes_.begin(), tastes_.end(), [](const Taste& a, const Taste& b) {
            return a.index.first < b.index.first;
        });

        AVLTree tree;
        std::optional<Taste> best;
        std::size_t i = 0;
        while (i < tastes_.size()) {
            const int x = tastes_[i].index.first;

            // The cut includes every topping with this x coordinate.
            while (i < tastes_.size() && tastes_[i].index.first == x) {
                const Taste& topping = tastes_[i];
                tree.insert(topping.index.second, topping.taste, topping.index);
                ++i;
            }

            std::optional<Taste> candidate = tree.tastiestSlice();
            if (candidate && (!best || candidate->taste > best->taste)) {
                candidate->index.first = x;
                best = candidate;
            }
        }
        return best;
    }
};
