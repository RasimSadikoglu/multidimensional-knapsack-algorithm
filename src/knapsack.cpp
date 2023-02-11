#include "knapsack.h"

#include <algorithm>

extern bool terminate;

mknapsack::MKnapsack::MKnapsack(
    std::vector<std::vector<uint32_t>> &items,
    std::vector<uint32_t> &limits,
    Greedy greedy,
    Utility utility
):
    items{items},
    limits{limits},
    greedy{greedy},
    utility{utility}
{
}

void mknapsack::MKnapsack::knapsack_solve()
{
    greedy.greedy_sort();

    int result = knapsack_dfs_mirror();

    utility.print_result(result, max_stack);
}

int mknapsack::MKnapsack::knapsack_dfs_mirror() {
    uint32_t limits_size = static_cast<uint32_t>(limits.size());
    uint32_t items_size = static_cast<uint32_t>(items.size());

    std::vector<uint32_t> weights(limits_size);

    uint32_t max_value = 0;

    std::vector<bool> stack(items_size);
    for (uint32_t i = 0; i < items_size; i++) stack[i] = 1;
    uint32_t si = 0;

    uint32_t si_max = 0;

    uint32_t value = 0;
    bool prev = 1;

    for (uint32_t i = 0; i < items_size; i++) {
        value += items[i][0];
        for (uint32_t j = 0; j < limits_size; j++) {
            weights[j] += items[i][j + 1];
        }
    }

    while (si < items_size && !terminate) {
        
        bool current = stack[si];

        if (!current) {
            value += items[si][0];
            for (uint32_t i = 0; i < limits_size; i++) weights[i] += items[si][i + 1];

            if (si > si_max) {
                si_max = si;
                utility.print_console(si_max + 1, max_value, items_size);
            }

            prev = current;
            stack[si] = 1;
            si++;
            continue;
        }

        auto tw = weights;

        bool isfit = true;
        for (uint32_t i = 0; i < limits_size; i++) {
            tw[i] -= items[si][i + 1];
            if (isfit) isfit = tw[i] <= limits[i];
        }

        if (isfit) {
            stack[si] = 0;
            value -= items[si][0];
            if (value > max_value) {
                max_value = value;
                max_stack = stack;
                utility.print_console(si_max + 1, max_value, items_size);
            }
            value += items[si][0];
            stack[si] = 1;
            if (prev) si = 0;
            else si++;
            prev = 0;
            continue;
        }
        
        stack[si] = 0;

        value -= items[si][0];
        weights = std::move(tw);

        // Update si.
        if (!prev) si = 0;
        else si++;

        prev = current;
    }

    // Push result into the items container.
    for (uint32_t i = 0; i < items_size; i++) items[i].push_back(max_stack[i]);

    // Sort the array in order to get initial order of the items.
    std::sort(items.begin(), items.end(), [](std::vector<uint32_t> v1, std::vector<uint32_t> v2) { return v1[v1.size() - 2] < v2[v2.size() - 2]; });
    for (uint32_t i = 0; i < items_size; i++) max_stack[i] = items[i].back();

    return max_value;
}
