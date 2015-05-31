/* [Twitter] http://www.careercup.com/question?id=5735322939817984

Objective: Write a function to find all the combinations of three numbers that sum to zero 
Sample input: 
[2, 3, 1, -2, -1, 0, 2, -3, 0] 
Sample output: 
2, -2, 0 
1, -1, 0 
3, -2, -1 
3, 0, -3 
3, 0, -3
*/

#pragma once
#include "util.h"
#include <vector>
#include <tuple>
#include <algorithm>

namespace SumToZero {
    using namespace std;

    size_t factorial(size_t n) {
        size_t res = 1;
        for (size_t i = n; i > 1; --i)
            res *= i;
        return res;
    }

    size_t binomialCoeff(size_t n, size_t k) {
        return factorial(n) / (factorial(k) * factorial(n - k));
    }

    using TPair = std::tuple<int, int>;
    using TPairs = std::vector<TPair>;

    TPairs Sum2(const vector<int> & v, size_t first, size_t last, int x) {
        TPairs res;
        while (first < last) {
            int sum = v[first] + v[last];
            if (sum > x) {
                --last;
            } else if (sum < x) {
                ++first;
            } else { // sum == x
                size_t k1 = 1;
                for (size_t i = first + 1; i < last && v[i] == v[first]; ++i)
                    ++k1;

                size_t k2 = 1;
                for (size_t i = last - 1; i > first && v[i] == v[last]; --i)
                    ++k2;

                size_t count = k1 + k2 <= last - first + 1 ? k1 * k2 : binomialCoeff(last - first + 1, 2);
                for (size_t i = 0; i < count; ++i) {
                    res.push_back(make_tuple(v[first], v[last]));
                }
                first += k1;
                last -= k2;
            }
        }
        return res;
    }

    using TTuple = std::tuple<int, int, int>;
    using TTuples = std::vector<TTuple>;

    TTuples Sum3(vector<int> & v) {
        sort(v.begin(), v.end());

        TTuples res;
        for (size_t i = 0; i < v.size() - 1; ++i) {
            int x = v[i];

            auto pairs = Sum2(v, i + 1, v.size() - 1, -x);
            for (const auto & pair : pairs) {
                res.push_back(make_tuple(x, get<0>(pair), get<1>(pair)));
            }
        }
        return res;
    }


    void Run() {
        vector<vector<int>> vec = { {2, 3, 1, -2, -1, 0, 2, -3, 0},
                                    {0, 0, 0},
                                    {0, 0, 0, 0},
                                    {-5, 4, -2, -2, 1, 1, 0} };
        vector<TTuples> ans = { 
            {{-3, 0, 3}, {-3, 0, 3}, {-3, 1, 2}, {-3, 1, 2}, {-2, -1, 3}, {-2, 0, 2}, {-2, 0, 2}, {-2, 0, 2}, {-2, 0, 2}, {-1, 0, 1}, {-1, 0, 1}},
            {{0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{-5, 1, 4}, {-5, 1, 4}, {-2, -2, 4}, {-2, 1, 1}, {-2, 1, 1}}
        };

        util::Test(Sum3, vec, ans);
    }
}