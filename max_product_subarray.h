/* http://www.geeksforgeeks.org/maximum-product-subarray/
See also (for better alg): http://algods-cracker.blogspot.in/2015/05/maximum-product-subarray.html

Given an array that contains both positive and negative integers, find the product of the maximum product subarray.
Expected Time complexity is O(n) and only O(1) extra space can be used.

Examples:

Input: arr[] = {6, -3, -10, 0, 2}
Output:   180  // The subarray is {6, -3, -10}

Input: arr[] = {-1, -3, -10, 0, 60}
Output:   60  // The subarray is {60}

Input: arr[] = {-2, -3, 0, -2, -40}
Output:   80  // The subarray is {-2, -40}
*/

#pragma once

#include "util.h"

#include <vector>
#include <algorithm>

namespace MaxProductSubarray {
    using namespace std;

    int MaxProductSubarray(const vector<int>& v) {
        if (v.empty())
            return 0;

        int res = v[0];
        int maxp = v[0];
        int minp = v[0];

        for (size_t i = 1; i < v.size(); ++i) {
            int currMaxp = max(maxp * v[i], max(minp * v[i], v[i]));
            int currMinp = min(maxp * v[i], min(minp * v[i], v[i]));

            res = max(res, currMaxp);

            maxp = currMaxp;
            minp = currMinp;
        }

        return res;
    }

    void Run() {
        vector<vector<int>> v = { {6, -3, -10, 0, 2}
                                , {-1, -3, -10, 0, 60}
                                , {-2, -3, 0, -2, -40}
                                , {5, 0, 4, 2}
                                , {}
                                , {3} };
        vector<int> ans = {180, 60, 80, 8, 0, 3};

        util::Test(MaxProductSubarray, v, ans);
    }
}