#include <iostream>
#include <fstream>
#include <vector>
#include <map>

void knapsack_solve(const std::vector<std::vector<int>> &items, const std::vector<int> &limits);

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "Wrong Usage!\nUsage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1], std::ifstream::in);

    int number_of_knapsacks, number_of_items;
    input >> number_of_knapsacks >> number_of_items;

    std::vector<std::vector<int>> items(number_of_items);
    for (int i = 0; i < number_of_items; i++) {
        int val;
        input >> val;
        items[i].push_back(val);
    }

    std::vector<int> knapsack_limits(number_of_knapsacks);
    for (auto &i: knapsack_limits) input >> i;

    for (int i = 0; i < number_of_knapsacks; i++) {
        for (int j = 0; j < number_of_items; j++) {
            int val;
            input >> val;
            items[j].push_back(val);
        }
    }

    knapsack_solve(items, knapsack_limits);

    return 0;
}

void knapsack_solve(const std::vector<std::vector<int>> &items, const std::vector<int> &limits) {

}