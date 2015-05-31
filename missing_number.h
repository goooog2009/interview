/* [Twitter] http://www.careercup.com/question?id=5670081354792960

You have n - 1 numbers from 1 to n. Your task is to find the missing number. 

I.e. 
n = 5 
v = [4, 2, 5, 1] 
The result is 3.
*/

#pragma once
#include "util.h"
#include <vector>

namespace MissingNumber {
    using namespace std;

    size_t FindMissingNumber(vector<int> & v) {
        // Make vector 0-based
        for (auto & x : v) {
            x -= 1;
        }

        // Add placeholder for missing element
        v.push_back(-1);

        size_t idx = 0;
        while (idx < v.size()) {
            if (idx == v[idx] || v[idx] == -1) {
                ++idx;
                continue;
            }

            swap(v[idx], v[v[idx]]);
        }

        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i] == -1)
                return i + 1;
        }

        // Should never happen
        return 0;
    }

    void Run() {
        vector<vector<int>> vec = { { 4, 2, 5, 1 }, { 5, 4, 2, 1 }, { 2, 3, 4 }, { 2 }, { 1, 2 } };
        vector<int> ans = { 3, 3, 1, 1, 3 };

        util::Test(FindMissingNumber, vec, ans);
    }
}