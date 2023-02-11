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