/*
Find length of longest increasing subsequence in array.

See:
    * https://www.youtube.com/watch?v=CE2b_-XfVDk
    * http://www.geeksforgeeks.org/dynamic-programming-set-3-longest-increasing-subsequence/
    * http://www.geeksforgeeks.org/longest-monotonically-increasing-subsequence-size-n-log-n/
*/

#pragma once

#include "util.h"

#include <algorithm>
#include <vector>

namespace LongestIncreasingSubsequence {
    using namespace std;

    // Dynamic programming algorithm, O(n^2)
    size_t LIS_dynamic(const vector<int> & v) {
        if (v.empty())
            return 0;

        vector<size_t> length(v.size(), 1);

        for (size_t i = 1; i < v.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (v[j] < v[i])
                    length[i] = max(length[j] + 1, length[i]);
            }
        }

        return *max_element(length.begin(), length.end());
    }

    // O(n log n) algorithm
    size_t LIS_optimal(const vector<int> & v) {
        if (v.empty())
            return 0;

        // Auxiliary array, index is lengh of subsequence, value is the last (maximum) element of the subsequence.
        // Important note: end element of smaller list is smaller than end elements of larger lists.
        vector<int> lastElem(1, v[0]);
        
        for (const auto & i : v) {
            auto it = lower_bound(lastElem.begin(), lastElem.end(), i);

            // 1. If A[i] is smallest among all end candidates of active lists, we will start new active list of length 1.
            if (it == lastElem.begin())
                lastElem.front() = i;
            // 2. If A[i] is largest among all end candidates of active lists, we will clone the largest active list, and extend it by A[i].
            else if (it == lastElem.end())
                lastElem.push_back(i);
            // 3. If A[i] is in between, we will find a list with largest end element that is smaller than A[i].
            //    Clone and extend this list by A[i]. We will discard all other lists of same length as that of this modified list.
            else
                *it = i;
        }

        return lastElem.size();
    }

    void Run() {
        vector<vector<int>> v = { { 10, 22, 9, 33, 21, 50, 41, 60, 80 }
                                , { 3, 4, -1, 0, 6, 2, 3 }
                                , { 1, 4, 3, 2, -1, 5 }
                                , { 2, 5, 1, 8, 3 }
                                , { 0 }
                                , {} };
        vector<size_t> ans = { 6
                             , 4
                             , 3
                             , 3
                             , 1
                             , 0 };

        util::Test(LIS_dynamic, v, ans);
        util::Test(LIS_optimal, v, ans);
    }
}