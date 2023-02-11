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

volatile bool terminate = false;

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

int knapsack_dfs(vector<vector<int>> items, const vector<int> &limits, bool max_stack[]) {

    for (size_t i = 0; i < items.size(); i++) {
        float val = 0;
        for (size_t j = 0; j < limits.size(); j++) {
            val += (float)items[i][j + 1] / limits[j];
        }
        val /= limits.size();
        val = items[i][0] / val;
        items[i].push_back(val);
    }

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
    bool prev = 0;

    while (si != -1 && !terminate) {

        bool top = stack[si];

        if (top) {
            value -= items[si][0];
            for (int i = 0; i < limits_size; i++) weights[i] -= items[si][i + 1];
            if (si < si_min) {
                si_min = si;
                print_console(si_min, max_value, items_size);
            }
            prev = top;
            stack[si] = 0;
            si--;
            continue;
        }

        int tw[limits_size];
        memcpy(tw, weights, sizeof(int) * limits_size);
        
        bool isfit = true;
        for (int i = 0; i < limits_size && isfit; i++) {
            tw[i] += items[si][i + 1];
            isfit = tw[i] <= limits[i];
        }

        if (!isfit) {
            if (!prev) si = items_size - 1;
            else si--;
            prev = 1;
            continue;
        }

        stack[si] = 1;

        value += items[si][0];
        memcpy(weights, tw, sizeof(int) * limits_size);

        if (value > max_value) {
            max_value = value;
            memcpy(max_stack, stack, items_size * sizeof(bool));
            print_console(si_min, max_value, items_size);
        }

        if (prev) si = items_size - 1;
        else si--;

        prev = top;
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
    return;
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