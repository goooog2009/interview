/*
Given 2 strings find longest common substring.

Example:
    "abcdaf"
    "zbcdf"

Answer:
    "bcd"

References:
    * Algorithm description using dynamic programming in O(n * m): https://www.youtube.com/watch?v=BysNXJHzCEs
    * Faster yet more complex implementation using suffix tree: http://en.wikipedia.org/wiki/Longest_common_substring_problem
*/

#pragma once

#include <string>
#include <vector>

namespace {
    using namespace std;

    // Dymanic programming algorithm
    // O(n * m) time and memory
    string LongestCommonSubstring(const string& a, const string& b) {
        size_t rows = a.size() + 1;
        size_t cols = b.size() + 1;

        vector<size_t> v(rows * cols, 0); // Ideally would initialize only 1st row and column
        size_t* pv = &*v.begin();

        size_t maxLen = 0;
        size_t maxIdx = 0;

        for (size_t i = 1; i < rows; ++i) {
            for (size_t j = 1; j < cols; ++j) {
                if (a[i - 1] != b[j - 1]) {
                    pv[i * j + j] = 0;
                    continue;
                }

                pv[i * j + j] = pv[(i - 1)*(j - 1) + j - 1] + 1;

                if (pv[i * j + j] > maxLen) {
                    maxLen = pv[i * j + j];
                    maxIdx = i - 1;
                }
            }
        }

        return maxLen > 0 ? a.substr(maxIdx - maxLen + 1, maxLen) : "";
    }
}

string LongestCommonSubstring2(const string& a, const string& b) {
    vector<size_t> prev(b.size() + 1, 0);
    vector<size_t> curr(b.size() + 1, 0);

    size_t maxLen = 0;
    size_t maxIdx = 0;

    for (size_t i = 0; i < a.size(); ++i) {
        prev.swap(curr);

        for (size_t j = 0; j < b.size(); ++j) {
            if (a[i] != b[j]) {
                curr[j + 1] = 0;
                continue;
            }

            curr[j + 1] = prev[j] + 1;
            if (curr[j + 1] > maxLen) {
                maxLen = curr[j + 1];
                maxIdx = j;
            }
        }
    }

    return maxLen ? b.substr(maxIdx - maxLen + 1, maxLen) : "";
}

void TestLongestCommonSubstring() {
    util::TestOne<string, string>("case 1", LongestCommonSubstring("abcdaf", "zbcdf"), "bcd");
    util::TestOne<string, string>("case 1", LongestCommonSubstring("", ""), "");
    util::TestOne<string, string>("case 1", LongestCommonSubstring("abcdafggghhkkl", "zbcdfgghhreef"), "gghh");

    util::TestOne<string, string>("case 1", LongestCommonSubstring2("abcdaf", "zbcdf"), "bcd");
    util::TestOne<string, string>("case 1", LongestCommonSubstring2("", ""), "");
    util::TestOne<string, string>("case 1", LongestCommonSubstring2("abcdafggghhkkl", "zbcdfgghhreef"), "gghh");

}