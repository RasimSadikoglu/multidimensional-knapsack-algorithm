#pragma once

#include <vector>
#include <string>

#include "greedy.h"
#include "utility.h"

namespace mknapsack {
    class MKnapsack {
    private:
        std::vector<std::vector<uint32_t>> &items;
        std::vector<uint32_t> &limits;
        std::vector<bool> max_stack;
        Greedy greedy;
        Utility utility;
    private:
        int knapsack_dfs_mirror();
    public:
        MKnapsack(
            std::vector<std::vector<uint32_t>> &items,
            std::vector<uint32_t> &limits,
            const Greedy greedy,
            const Utility utility
        );
        void knapsack_solve();
    };
}