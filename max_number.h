/* [Amazon]
http://www.careercup.com/question?id=5645040497983488

Given a Integer, find the maximum number that can be formed from the digits. 
Input : 8754365 
output : 8765543 
*/

#pragma once
#include <array>
#include <vector>
#include <iostream>

namespace MaxNumber {
    using namespace std;

    int MaxNumber(int n) {
        bool positive = n > 0;

        if (!positive)
            n = -n;

        array<size_t, 10> counts = {};
        while (n) {
            ++counts[n % 10];
            n /= 10;
        }

        int res = 0;
        for (size_t i = 0; i < counts.size(); ++i) {
            size_t digit = positive ? counts.size() - i - 1 : i;
            size_t count = counts[digit];

            while (count) {
                res = res*10 + digit;
                --count;
            }
        }

        return positive ? res : -res;
    }

    void Test(int n, int ans, int res) {
        if (res != ans)
            cout << "FAIL n=" << n << " res=" << res << " ans=" << ans << endl;
        else
            cout << "OK" << endl;
    }


    void Run() {
        vector<int> nums = { 8754365, 0, 12, -12, -333, -321, 111 };
        vector<int> ans = { 8765543, 0, 21, -12, -333, -123, 111 };

        for (size_t i = 0; i < nums.size(); ++i) {
            Test(nums[i], ans[i], MaxNumber(nums[i]));
        }
    }
}