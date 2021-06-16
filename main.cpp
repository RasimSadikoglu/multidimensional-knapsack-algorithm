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

void knapsack_solve(vector<vector<int>> items, const vector<int> &limits, std::string file_name);
int knapsack_dfs_mirror(vector<vector<int>> items, const vector<int> &limits, bool max_stack[]);
void greedy_sort(vector<vector<int>> &items, const vector<int> &limits);
bool sortf(vector<int> v1, vector<int> v2);
void handler(int num);
void print_console(int stage, int max, int max_stage);

// For signal handler, signal handler will make this true whenever user presses CTRL+C.
volatile bool terminate = false;

int main(int argc, char **argv) {

    signal(SIGINT, handler);

    // Input Section
    std::ifstream input;
    
    // Open File
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

    if (!input.is_open()) {
        std::cout << "File not found!\n";
        exit(EXIT_FAILURE);
    }

    int number_of_knapsacks, number_of_items;
    input >> number_of_knapsacks >> number_of_items;

    // Get items' value.
    vector<vector<int>> items(number_of_items);
    for (int i = 0; i < number_of_items; i++) {
        int value;
        input >> value;
        items[i].push_back(value);
    }

    // Get knapsacks' limit
    vector<int> knapsack_limits(number_of_knapsacks);
    for (auto &i: knapsack_limits) input >> i;

    // Get items' weights
    for (int i = 0; i < number_of_knapsacks; i++) {
        for (int j = 0; j < number_of_items; j++) {
            int weight;
            input >> weight;
            items[j].push_back(weight);
        }
    }

    input.close();

    // Start the timer for the algorithm.
    auto start_time = std::chrono::high_resolution_clock::now();
    knapsack_solve(items, knapsack_limits);
    auto end_time = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Execution Time: " << duration.count() << "sec." << std::endl;

    return 0;
}

// Helper function for the real algorithm.
void knapsack_solve(vector<vector<int>> items, const vector<int> &limits, std::string file_name) {

    // Sort the array by their greediness score.
    greedy_sort(items, limits);

    // Create a boolean array for result.
    bool max_stack[items.size()] {};
    int result = knapsack_dfs_mirror(items, limits, max_stack);

    // Print the results to the stdout.
    std::cout << result << std::endl;
    for (size_t i = 0; i < items.size(); i++) std::cout << max_stack[i];
    std::cout << std::endl;

    // Create result file.
    std::ofstream output(file_name.substr(0, file_name.size() - 4) + "_output.txt", std::ofstream::out);
    output << result << std::endl;
    for (size_t i = 0; i < items.size(); i++) output << max_stack[i] << std::endl;
    output.close();

    return;
}

int knapsack_dfs_mirror(vector<vector<int>> items, const vector<int> &limits, bool max_stack[]) {
    // Save items' and limits' size for easy access.
    int limits_size = limits.size();
    int items_size = items.size();

    // Current weights of the knapsacks.
    int weights[limits_size] {};

    // Maximum value has ever found while executing.
    int max_value = 0;

    // State of the items, true means contains, false means doesn't.
    bool stack[items_size] {};
    for (int i = 0; i < items_size; i++) stack[i] = 1;
    int si = 0;

    // No effect for the algorithm, only for the keeping track the stage.
    int si_max = 0;

    // Current knapsacks' value and previous value of the si.
    int value = 0;
    bool prev = 1;

    // Add all the items into the knapsacks.
    for (int i = 0; i < items_size; i++) {
        value += items[i][0];
        for (int j = 0; j < limits_size; j++) {
            weights[j] += items[i][j + 1];
        }
    }

    // Main loop for dfs.
    while (si < items_size && !terminate) {
        
        // Print stack for debug.
        /*for (int i = 0; i < items_size; i++) std::cout << stack[i];
        std::cout << "\n";*/

        // Current value of the stack that si pointing.
        bool current = stack[si];

        // If current is false add that item into the knapsack.
        if (!current) {
            value += items[si][0];
            for (int i = 0; i < limits_size; i++) weights[i] += items[si][i + 1];

            if (si > si_max) {
                si_max = si;
                print_console("Mirror DFS", si_max + 1, max_value, items_size);
            }

            prev = current;
            stack[si] = 1;
            si++;
            continue;
        }

        // Create a temporary container for weights.
        int tw[limits_size];
        memcpy(tw, weights, sizeof(int) * limits_size);

        // Check for that popping makes the knapsacks valid.
        bool isfit = true;
        for (int i = 0; i < limits_size; i++) {
            tw[i] -= items[si][i + 1];
            if (isfit) isfit = tw[i] <= limits[i];
        }

        // If knapsacks are valid, check for the maximum value.
        if (isfit) {
            stack[si] = 0;
            value -= items[si][0];
            if (value > max_value) {
                max_value = value;
                memcpy(max_stack, stack, items_size * sizeof(bool));
                print_console("Mirror DFS", si_max + 1, max_value, items_size);
            }
            value += items[si][0];
            stack[si] = 1;
            if (prev) si = 0;
            else si++;
            prev = 0;
            continue;
        }
        
        // Delete that item from the knapsacks.
        stack[si] = 0;

        // Update weights and value according to changes have been made.
        value -= items[si][0];
        memcpy(weights, tw, sizeof(int) * limits_size);

        // Update si.
        if (!prev) si = 0;
        else si++;

        prev = current;
    }

    // Push result into the items container.
    for (size_t i = 0; i < items.size(); i++) items[i].push_back(max_stack[i]);

    // Sort the array in order to get initial order of the items.
    std::sort(items.begin(), items.end(), sortf);
    for (int i = 0; i < items_size; i++) max_stack[i] = items[i].back();

    return max_value;
}

void greedy_sort(vector<vector<int>> &items, const vector<int> &limits) {
    /*
    GREEDY PART
    */
    // The program could be optimized using a better greedy algorithm.
    for (size_t i = 0; i < items.size(); i++) {
        float val = 0;
        for (size_t j = 0; j < limits.size(); j++) {
            val += (float)items[i][j + 1] / limits[j];
        }
        val /= limits.size();
        val = items[i][0] / val;
        items[i].push_back(val);
    }
    /*
    GREEDY PART
    */

    // Sort the array after adding their initial indexes.
    for (size_t i = 0; i < items.size(); i++) items[i].push_back(i);
    std::sort(items.begin(), items.end(), sortf);

    // Save number of items and number of knapsacks for easy access.
    int limits_size = limits.size();
    int items_size = items.size();

    // Current state of the knapsacks.
    int weights[limits_size] {};

    // Maximum value for knapsack.
    int max_value = 0;

    // A boolean array for depth first search and an index for it.
    bool stack[items_size] {};
    int si = items_size - 1;

    // This is only for stage. Doesn't related with result or solution.
    int si_min = si + 1;

    // Current value and previous stack value.
    int value = 0;
    bool prev = 0;

    // Main loop for traversing the tree.
    while (si != -1 && !terminate) {

        // Current value of stack at index si.
        bool current = stack[si];

        // Popping the item for the knapsacks.
        if (current) {
            value -= items[si][0];
            for (int i = 0; i < limits_size; i++) weights[i] -= items[si][i + 1];

            // Check for stage.
            if (si < si_min) {
                si_min = si;
                print_console(si_min, max_value, items_size);
            }

            prev = current;
            stack[si] = 0;
            si--;
            continue;
        }

        // Take a copy of weights array.
        int tw[limits_size];
        memcpy(tw, weights, sizeof(int) * limits_size);
        
        // Check for current item can fit into the knapsack.
        bool isfit = true;
        for (int i = 0; i < limits_size && isfit; i++) {
            tw[i] += items[si][i + 1];
            isfit = tw[i] <= limits[i];
        }

        // If It can't fit into the knapsack...
        if (!isfit) {
            terminate = true;
            // Update si according to prev.
            if (!prev) si = items_size - 1;
            else si--;
            prev = 1;
            continue;
        }

        stack[si] = 1;

        // Update weights array.
        value += items[si][0];
        memcpy(weights, tw, sizeof(int) * limits_size);

        // Check for the current path is better than previous solution.
        if (value > max_value) {
            max_value = value;
            memcpy(max_stack, stack, items_size * sizeof(bool));
            print_console(si_min, max_value, items_size);
        }

        // Update si according to prev.
        if (prev) si = items_size - 1;
        else si--;

        prev = current;
    }

    // Sort the array by their order that given in the input file.
    for (size_t i = 0; i < items.size(); i++) items[i].push_back(max_stack[i]);

    std::sort(items.begin(), items.end(), sortf);
    for (int i = 0; i < items_size; i++) max_stack[i] = items[i].back();

    return max_value;
}

// Function for sorting the vector.
bool sortf(vector<int> v1, vector<int> v2) {
    return v1[v1.size() - 2] < v2[v2.size() - 2];
}

// Signal handler.
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