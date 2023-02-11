#include "greedy.h"
#include "utility.h"

#include <cstdlib>
#include <algorithm>

mknapsack::Greedy::Greedy(std::vector<std::vector<uint32_t>> &items, std::vector<uint32_t> &limits):
    items{items},
    limits{limits}
{
}

void mknapsack::Greedy::greedy_sort()
{

    for (size_t i = 0; i < items.size(); i++) {
        float val = 0;
        for (size_t j = 0; j < limits.size(); j++) {
            val += static_cast<float>(items[i][j + 1]) * static_cast<float>(limits.size()) / static_cast<float>(limits[j]);
        }
        val = static_cast<float>(items[i][0]) / val;
        items[i].push_back(static_cast<unsigned>(val));
    }

    for (uint32_t i = 0; i < items.size(); i++) items[i].push_back(i);
    std::sort(items.begin(), items.end(), [](std::vector<uint32_t> v1, std::vector<uint32_t> v2) { return v1[v1.size() - 2] < v2[v2.size() - 2]; });

    return;
}