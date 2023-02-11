#define UNUSED __attribute_maybe_unused__

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <signal.h>

#include "knapsack.h"

volatile bool terminate = false;

int main(int argc, char **argv) {

    if (argc != 2) exit(EXIT_FAILURE);

//  #REGION IO
    std::ifstream input;
    
    input.open("samples/" + std::string(argv[1]), std::ifstream::in);

    uint32_t number_of_knapsacks, number_of_items;
    input >> number_of_knapsacks >> number_of_items;

    std::vector<std::vector<uint32_t>> items(number_of_items);
    for (uint32_t i = 0; i < number_of_items; i++) {
        uint32_t value;
        input >> value;
        items[i].push_back(value);
    }

    std::vector<uint32_t> knapsack_limits(number_of_knapsacks);
    for (auto &i: knapsack_limits) input >> i;

    for (uint32_t i = 0; i < number_of_knapsacks; i++) {
        for (uint32_t j = 0; j < number_of_items; j++) {
            uint32_t weight;
            input >> weight;
            items[j].push_back(weight);
        }
    }

    input.close();
//  #REGION IO: END    

    signal(SIGINT, [](UNUSED int num) { terminate = true; });
    std::cout << "\033[s";

    auto knapsack = mknapsack::MKnapsack(
        items, 
        knapsack_limits,
        mknapsack::Greedy(items, knapsack_limits),
        mknapsack::Utility(argv[1], items)
    );

    auto start_time = std::chrono::high_resolution_clock::now();
    knapsack.knapsack_solve();
    auto end_time = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Execution Time: " << duration.count() << "sec." << std::endl;

    return 0;
}