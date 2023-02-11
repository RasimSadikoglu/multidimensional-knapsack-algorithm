#define UNUSED __attribute_maybe_unused__

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include <stack>
#include <cstring>
#include <algorithm>
#include <signal.h>

#include "knapsack.h"

volatile bool terminate = false;

int main(int argc, char **argv) {

    signal(SIGINT, [](UNUSED int num) { terminate = true; });

//  #REGION IO
    std::ifstream input;
    
    if (argc == 1) {
        std::string file_name;
        std::cout << "Please enter the filename: ";
        std::cin >> file_name;

        input.open(file_name, std::ifstream::in);
    } else if (argc == 2) {
        file_name = argv[1];
        input.open("samples/" + file_name, std::ifstream::in);
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

    std::vector<std::vector<uint32_t>> items(number_of_items);
    for (int i = 0; i < number_of_items; i++) {
        int value;
        input >> value;
        items[i].push_back(value);
    }

    std::vector<uint32_t> knapsack_limits(number_of_knapsacks);
    for (auto &i: knapsack_limits) input >> i;

    for (int i = 0; i < number_of_knapsacks; i++) {
        for (int j = 0; j < number_of_items; j++) {
            int weight;
            input >> weight;
            items[j].push_back(weight);
        }
    }

    input.close();
//  #REGION IO: END    

    auto knapsack = mknapsack::MKnapsack(
        items, 
        knapsack_limits,
        mknapsack::Greedy(items, knapsack_limits),
        mknapsack::Utility(file_name, items)
    );

    auto start_time = std::chrono::high_resolution_clock::now();
    knapsack.knapsack_solve();
    auto end_time = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Execution Time: " << duration.count() << "sec." << std::endl;

    return 0;
}