#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "item.h"

extern int errno;

#define FILE_NAME_SIZE 50

// noi: (N)umber (o)f (I)tems - now: (N)umber (o)f (W)eights
void knapsack_solution(item items[], int *weight_limits, int noi, int now);

int main(int argc, char **argv) {

    errno = 0;

    // Open File
    FILE *input_file;

    if (argc == 1) {
        char file_name[FILE_NAME_SIZE];
        printf("Please enter the file name: ");
        scanf("%s", file_name);

        input_file = fopen(file_name, "r");
    } else if (argc == 2) {
        input_file = fopen(argv[1], "r");
    } else {
        fprintf(stderr, "Wrong Usage!\nUsage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (input_file == NULL) {
        fprintf(stderr, "%d: %s\n", errno, strerror(errno));
        return errno;
    }

    // Take inputs from the file
    int num_of_weights, num_of_items;
    fscanf(input_file, "%d %d", &num_of_weights, &num_of_items);

    item items[num_of_items];

    for (int i = 0; i < num_of_items; i++) {
        int value;
        fscanf(input_file, "%d", &value);
        
        items[i] = item_create(value, num_of_weights);
    }

    int weight_limits[num_of_weights];
    for (int i = 0; i < num_of_weights; i++) {
        fscanf(input_file, "%d", weight_limits + i);
    }

    for (int i = 0; i < num_of_weights; i++) {
        for (int j = 0; j < num_of_items; j++) {
            fscanf(input_file, "%d", &(items[j].weights[i]));
        }
    }

    fclose(input_file);

    // Solution
    knapsack_solution(items, weight_limits, num_of_items, num_of_weights);

    // Free memory before quit
    for (int i = 0; i < num_of_items; i++) {
        item_free(items + i);
    }

    return 0;
}

void knapsack_solution(item items[], int weight_limits[], int noi, int now) {

    /*


    Solution


    */

    return;
}