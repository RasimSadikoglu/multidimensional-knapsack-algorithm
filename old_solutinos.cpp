/*
Bekir Nazmi Görkem 150118017
Burak Çağlayan 150118027
Rasim Sadıkoğlu 150118009
*/

/*
This algorithms don't work with large samples. With small samples It produces the optimal solution.
The problem is the memory. Other than that algorithms are correct.
*/

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

/*
This is the old version of the dfs.
*/
int knapsack_dfs(vector<vector<int>> items, const vector<int> &limits, bool max_stack[]) {
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
                print_console("DFS", items_size - si_min, max_value, items_size);
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
            //terminate = true;
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
            print_console("DFS", items_size - si_min, max_value, items_size);
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