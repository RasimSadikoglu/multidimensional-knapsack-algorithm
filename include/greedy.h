#pragma once

#include <vector>
#include <cstdint>

namespace mknapsack {
    class Greedy {
    private:
        std::vector<std::vector<uint32_t>> &items;
        std::vector<uint32_t> &limits;
    public:
        Greedy(
            std::vector<std::vector<uint32_t>> &items,
            std::vector<uint32_t> &limits
        );
        void greedy_sort();
    };
}