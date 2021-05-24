#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>

using std::vector;
using std::map;

void knapsack_solve(const vector<vector<int>> &items, const vector<int> &limits);
int knapsack_helper(const vector<vector<int>> &items, map<vector<int>, int> &table, const vector<int> weights);

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "Wrong Usage!\nUsage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1], std::ifstream::in);

    int number_of_knapsacks, number_of_items;
    input >> number_of_knapsacks >> number_of_items;

    vector<vector<int>> items(number_of_items);
    for (int i = 0; i < number_of_items; i++) {
        int val;
        input >> val;
        items[i].push_back(val);
    }

    vector<int> knapsack_limits(number_of_knapsacks);
    for (auto &i: knapsack_limits) input >> i;

    for (int i = 0; i < number_of_knapsacks; i++) {
        for (int j = 0; j < number_of_items; j++) {
            int val;
            input >> val;
            items[j].push_back(val);
        }
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    knapsack_solve(items, knapsack_limits);
    auto end_time = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Execution Time: " << duration.count() << "sec." << std::endl;

    return 0;
}

void knapsack_solve(const vector<vector<int>> &items, const vector<int> &limits) {
    map<vector<int>, int> table;

    vector<int> weights(limits);

    weights.push_back(items.size() - 1);

    vector<int> res_items;
    int result = knapsack_helper(items, table, weights);

    std::cout << result << std::endl;

    return;
}

int knapsack_helper(const vector<vector<int>> &items, map<vector<int>, int> &table, const vector<int> weights) {

    if (weights.back() == -1) return 0;

    if (table[weights] != 0) return table[weights] == -1 ? 0 : table[weights];

    vector<int> cweights(weights);
    cweights[cweights.size() - 1]--;
    int p1 = knapsack_helper(items, table, cweights);

    for (size_t i = 0; i < cweights.size() - 1; i++) {
        if (cweights[i] - items[weights.back()][i + 1] < 0) {
            if (p1 == 0) table[weights] = -1;
            else table[weights] = p1;
            return p1;
        }
        cweights[i] -= items[weights.back()][i + 1];
    }

    int p2 = knapsack_helper(items, table, cweights) + items[weights.back()][0];

    if (p2 >= p1) table[weights] = p2;
    else table[weights] = p1;

    if (table[weights] == 0) table[weights] = -1;

    return table[weights];
}