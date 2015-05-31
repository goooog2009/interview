/* http://www.geeksforgeeks.org/maximum-of-all-subarrays-of-size-k/

Given an array and an integer k, find the maximum for each and every contiguous subarray of size k.

Examples:

Input :
arr[] = {1, 2, 3, 1, 4, 5, 2, 3, 6}
k = 3
Output :
3 3 4 5 5 5 6

Input :
arr[] = {8, 5, 10, 7, 9, 4, 15, 12, 90, 13}
k = 4
Output :
10 10 10 15 15 90 90
*/

#pragma once

#include "util.h"

#include <vector>
#include <deque>

namespace MaxOfAllSubarrays {
    using namespace std;

    vector<int> MaxOfAllSubarrays(const vector<int>& v, size_t k) {
        deque<int> d;
        for (size_t i = 0; i < k; ++i) {
            while (!d.empty() && v[d.back()] <= v[i]) {
                d.pop_back();
            }

            d.push_back(i);
        }

        vector<int> vmax;
        for (size_t i = k; i < v.size(); ++i) {
            vmax.push_back(v[d.front()]);

            while (!d.empty() && v[d.back()] <= v[i]) {
                d.pop_back();
            }

            while (!d.empty() && (i - d.front()) >= k) {
                d.pop_front();
            }

            d.push_back(i);
        }
        vmax.push_back(v[d.front()]);

        return vmax;
    }

    void Run() {
        vector<vector<int>> v = { {1, 2, 3, 1, 4, 5, 2, 3, 6}
                                , {8, 5, 10, 7, 9, 4, 15, 12, 90, 13} };
        vector<size_t> k = {3, 4};
        vector<vector<int>> ans = { {3, 3, 4, 5, 5, 5, 6}
                                  , {10, 10, 10, 15, 15, 90, 90} };

        util::TestOne("case 0", MaxOfAllSubarrays(v[0], k[0]), ans[0]);
        util::TestOne("case 1", MaxOfAllSubarrays(v[1], k[1]), ans[1]);
    }

}