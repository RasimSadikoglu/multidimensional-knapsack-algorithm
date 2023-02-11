#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include <stack>

using std::vector;
using std::map;

void knapsack_solve(const vector<vector<int>> &items, const vector<int> &limits);
int knapsack_dp(const vector<vector<int>> &items, map<vector<int>, int> &table, vector<int> &weights);
int knapsack_dpi(const vector<vector<int>> &items, map<vector<int>, int> &table, vector<int> weights);
int knapsack_backtracking(const vector<vector<int>> &items, const vector<int> &limits);

int main(int argc, char **argv) {

    std::ifstream input;
    
    if (argc == 1) {
        std::string file_name;
        std::cout << "Please enter the filename: ";
        std::cin >> file_name;

        input.open(file_name, std::ifstream::in);
    } else if (argc == 2) {
        input.open(argv[1], std::ifstream::in);
    } else {
        std::cout << "Wrong Usage!\nUsage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    int number_of_knapsacks, number_of_items;
    input >> number_of_knapsacks >> number_of_items;

    vector<vector<int>> items(number_of_items);
    for (int i = 0; i < number_of_items; i++) {
        int value;
        input >> value;
        items[i].push_back(value);
    }

    vector<int> knapsack_limits(number_of_knapsacks);
    for (auto &i: knapsack_limits) input >> i;

    for (int i = 0; i < number_of_knapsacks; i++) {
        for (int j = 0; j < number_of_items; j++) {
            int weight;
            input >> weight;
            items[j].push_back(weight);
        }
    }

    input.close();

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
    int result = knapsack_backtracking(items, limits);

    std::cout << result << std::endl;

    return;
}

// Recursive DP
int knapsack_dp(const vector<vector<int>> &items, map<vector<int>, int> &table, vector<int> &weights) {

    int *val, item;
    if ((item = weights.back()) == -1) return 0;

    val = &table[weights];
    vector<int> cweights(weights);
    //weights.clear();

    if (*val != 0) {
        //std::cout << weights.back() << std::endl;
        return *val == -1 ? 0 : *val;
    }
    
    cweights[cweights.size() - 1]--;
    int p1 = knapsack_dp(items, table, cweights);

    for (size_t i = 0; i < cweights.size() - 1; i++) {
        if (cweights[i] - items[item][i + 1] < 0) {
            if (p1 == 0) *val = -1;
            else *val = p1;
            return p1;
        }
        cweights[i] -= items[item][i + 1];
    }

    int p2 = knapsack_dp(items, table, cweights) + items[item][0];

    if (p2 >= p1) *val = p2;
    else *val = p1;

    if (*val == 0) *val = -1;

    return *val;
}

// Iterative DP
int knapsack_dpi(const vector<vector<int>> &items, map<vector<int>, int> &table, vector<int> weights) {
    
    std::stack<vector<int>> s;
    s.push(weights);
    do {
        vector<int> current = s.top();

        current[current.size() - 1]--;

        int v1 = 0, v2 = 0;
        if (current.back() == -1) {
            v1 = 0;
        } else if ((v1 = table[current]) == 0) {
            s.push(current);
            continue;
        }

        bool isv2 = true;
        for (size_t i = 0; i < current.size() - 1; i++) {
            current[i] -= items[current.back() + 1][i + 1];
            if (current[i] < 0) {
                isv2 = false;
                break;
            }
        }
        
        if (isv2 && current.back() == -1) {
            v2 = 0;
        } else if (isv2 && (v2 = table[current]) == 0) {
            s.push(current);
            continue;
        }

        if (v1 == -1) v1 = 0;
        if (v2 == -1) v2 = 0;

        if (isv2) v2 += items[current.back() + 1][0];

        int *val = &table[s.top()];
        *val = v1;
        if (isv2) *val = v2 > v1 ? v2 : v1;
        if (*val == 0) *val = -1;

        s.pop();

    } while (!s.empty());

    return table[weights];
}

int knapsack_backtracking(const vector<vector<int>> &items, const vector<int> &limits) {

    std::vector<int> weights(limits.size());
    int max_value = 0;
    std::stack<bool> max_stack;

    std::stack<bool> s;
    for (size_t i = 0; i < items.size(); i++) s.push(false);
    max_stack = s;

    int value = 0;
    while (!s.empty()) {
        bool top = s.top();

        if (top) {
            value -= items[s.size() - 1][0];
            for (size_t i = 0; i < weights.size(); i++) weights[i] -= items[s.size() - 1][i + 1];
            s.pop();
            continue;
        }

        s.pop();
        s.push(1);

        vector<int> tw(weights);
        bool isfit = true;
        for (size_t i = 0; i < tw.size() && isfit; i++) {
            tw[i] += items[s.size() - 1][i + 1];
            isfit = tw[i] <= limits[i];
        }

        if (!isfit) {
            s.pop();
            continue;
        }

        value += items[s.size() - 1][0];
        weights = tw;

        for (size_t i = s.size(); i < items.size(); i++) s.push(0);

        if (value >= max_value) {
            max_value = value;
            max_stack = s;
        }
    }

    return max_value;
}