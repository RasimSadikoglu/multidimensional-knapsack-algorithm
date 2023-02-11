#pragma once

#include <vector>
#include <string>

namespace mknapsack {
    class Utility {
    private:
        const std::string file_name;
        std::vector<std::vector<uint32_t>> &items;
    public:
        Utility(
            const std::string file_name,
            std::vector<std::vector<uint32_t>> &items
        );
        void print_console(uint32_t stage, uint32_t max, uint32_t max_stage);
        void print_result(uint32_t result, std::vector<bool> &max_stack);
    };
}