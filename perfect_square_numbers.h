/* [Google] http://www.careercup.com/question?id=5725584103571456
Given a number "n", find the least number of perfect square numbers sum needed to get "n" 

Example: 
n = 12, return 3 (4 + 4 + 4) = (2^2 + 2^2 + 2^2) NOT (3^2 + 1 + 1 + 1) 
n = 6, return 3 (4 + 1 + 1) = (2^2 + 1^2 + 1^2)
*/

#pragma once

#include <iostream>
#include <vector>

namespace PerfectSquareNumbers {

using namespace std;

size_t CalcLeastSummands(size_t n) {
    vector<size_t> sums(max<size_t>(3, n) + 1);
    sums[0] = 0;
    sums[1] = 1;
    sums[2] = 2;

    for (size_t i = 3; i <= n; ++i) {
        size_t sq = sqrt(i);
        sums[i] = i - sq*sq + 1;
        for (size_t j = 1; j <= i/2; ++j) {
            if (sums[j] + sums[i - j] < sums[i])
                sums[i] = sums[j] + sums[i - j];
        }
    }

    return sums[n];
}

void CheckResult(size_t n, size_t res, size_t ans) {
    if (res != ans)
        cout << "ERROR! For N=" << n << ", res=" << res << ", corr=" << ans << endl;
    else
        cout << "OK. N=" << n << ", res=" << res << endl;
}

void Run() {
    cout << "PerfectSquareNumbers" << endl;

    vector<size_t> nums = {0, 1, 2, 4, 6, 8, 12, 18 };
    vector<size_t> ans  = {0, 1, 2, 1, 3, 2, 3,  2 };

    for (size_t i = 0; i < nums.size(); ++i) {
        CheckResult(nums[i], CalcLeastSummands(nums[i]), ans[i]);
    }
}

} // PerfectSquareNumbers