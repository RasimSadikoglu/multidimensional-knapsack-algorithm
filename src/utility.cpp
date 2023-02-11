#include "utility.h"

#include <iostream>
#include <fstream>

mknapsack::Utility::Utility(const std::string file_name, std::vector<std::vector<uint32_t>> &items):
    file_name{file_name},
    items{items}
{
}

void mknapsack::Utility::print_console(uint32_t stage, uint32_t max, uint32_t max_stage)
{

    std::cout << "\033[4F" << "Mirror DFS" << "\n";
    std::cout << "Current Stage: " << stage << "/" << max_stage << "\n";
    std::cout << "Current Maximum Value: " << max << "\n";
    std::cout << "Pressing CTRL+C will save current results." << "\n";

    return;
}

void mknapsack::Utility::print_result(uint32_t result, std::vector<bool> &max_stack) {
    std::cout << result << "\n";
    for (size_t i = 0; i < items.size(); i++) std::cout << max_stack[i];
    std::cout << "\n";

    std::ofstream output("results/" + file_name.substr(0, file_name.size() - 4) + "_output.txt", std::ofstream::out);
    output << result << "\n";
    for (size_t i = 0; i < items.size(); i++) output << max_stack[i] << "\n";
    output.close();
}
