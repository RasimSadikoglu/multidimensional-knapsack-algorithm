#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include <stack>
#include <cstring>
#include <algorithm>
#include <signal.h>

using std::vector;
using std::map;

void knapsack_solve(const vector<vector<int>> &items, const vector<int> &limits);
int knapsack_dp(const vector<vector<int>> &items, map<vector<int>, int> &table, vector<int> &weights);
int knapsack_dpi(const vector<vector<int>> &items, map<vector<int>, int> &table, vector<int> weights);
int knapsack_dfs(vector<vector<int>> items, const vector<int> &limits, bool max_stack[]);
bool sortf(vector<int> v1, vector<int> v2);
void handler(int num);
void print_console(int stage, int max, int max_stage);

bool terminate = false;

int main(int argc, char **argv) {

    signal(SIGINT, handler);

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

    bool max_stack[items.size()] {};
    int result = knapsack_dfs(items, limits, max_stack);

    std::cout << result << std::endl;
    for (size_t i = 0; i < items.size(); i++) std::cout << max_stack[i];
    std::cout << std::endl;

    std::ofstream output("output.txt", std::ofstream::out);
    output << result << std::endl;
    for (size_t i = 0; i < items.size(); i++) output << max_stack[i] << std::endl;
    output.close();

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

int knapsack_dfs(vector<vector<int>> items, const vector<int> &limits, bool max_stack[]) {

    for (size_t i = 0; i < items.size(); i++) items[i].push_back(i);
    std::sort(items.begin(), items.end(), sortf);

    int limits_size = limits.size();
    int items_size = items.size();

    int weights[limits_size] {};

    int max_value = 0;

    bool stack[items_size] {};
    int si = items_size - 1;

    int si_min = si + 1;

    int value = 0;
    while (si != -1 && !terminate) {

        bool top = stack[si];

        if (top) {
            value -= items[si][0];
            for (int i = 0; i < limits_size; i++) weights[i] -= items[si][i + 1];
            if (si < si_min) {
                si_min = si;
                print_console(si_min, max_value, items_size);
            }
            si--;
            continue;
        }

        stack[si] = 1;

        int tw[limits_size];
        memcpy(tw, weights, sizeof(int) * limits_size);
        
        bool isfit = true;
        for (int i = 0; i < limits_size && isfit; i++) {
            tw[i] += items[si][i + 1];
            isfit = tw[i] <= limits[i];
        }

        if (!isfit) {
            si--;
            continue;
        }

        value += items[si][0];
        memcpy(weights, tw, sizeof(int) * limits_size);

        for (si++; si < items_size; si++) stack[si] = 0;
        si--;

        if (value >= max_value) {
            max_value = value;
            memcpy(max_stack, stack, items_size * sizeof(bool));
            print_console(si_min, max_value, items_size);
        }
    }

    for (size_t i = 0; i < items.size(); i++) items[i].push_back(max_stack[i]);

    std::sort(items.begin(), items.end(), sortf);
    for (int i = 0; i < items_size; i++) max_stack[i] = items[i].back();

    return max_value;
}

bool sortf(vector<int> v1, vector<int> v2) {
    return v1[v1.size() - 2] < v2[v2.size() - 2];
}

void handler(int num) {
    std::cout << "Saving current results...\n";
    terminate = true;
}

void print_console(int stage, int max, int max_stage) {

    #if unix || __unix || __unix__
    system("clear");
    #elif _WIN32 || _WIN64
    system("cls");
    #endif
    
    std::cout << "Current Stage: " << max_stage - stage << "/" << max_stage << std::endl;
    std::cout << "Current Maximum Value: " << max << std::endl;
    std::cout << "Pressing CTRL+C will save current results." << std::endl;

    return;
}