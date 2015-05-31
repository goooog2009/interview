/* [Microsoft] http://algods-cracker.blogspot.com/

Given a 2D array, print it in spiral form.
*/

#pragma once

#include "util.h"
#include <vector>

namespace SpiralMatrixTraversal {
    using namespace std;

    vector<int> SpiralMatrixTraversal(const vector<vector<int>>& m) {
        vector<int> res;

        if (m.empty())
            return res;

        // We suppose m is valid matrix here.

        size_t rBeg = 0;
        size_t rEnd = m.size();

        size_t cBeg = 0;
        size_t cEnd = m.front().size();

        while (rBeg < rEnd && cBeg < cEnd) {
            // go right
            for (size_t i = cBeg; i < cEnd; ++i) {
                res.push_back(m[rBeg][i]);
            }
            ++rBeg;
            
            // go down
            for (size_t i = rBeg; i < rEnd; ++i) {
                res.push_back(m[i][cEnd - 1]);
            }
            --cEnd;

            // go left
            if (rBeg < rEnd) {
                for (size_t i = cEnd; i > cBeg; --i) {
                    res.push_back(m[rEnd - 1][i - 1]);
                }
                --rEnd;
            }

            // go up
            if (cBeg < cEnd) {
                for (size_t i = rEnd; i > rBeg; --i) {
                    res.push_back(m[i - 1][cBeg]);
                }
                ++cBeg;
            }
        }

        return res;
    }

    void Run() {
        vector<vector<vector<int>>> v = {
            {{}},
            
            {{0}},

            {{0, 1, 2, 3, 4}},

            {{0}
            ,{1}
            ,{2}
            ,{3}
            ,{4}},
            
            {{0, 1}
            ,{2, 3}},
            
            {{0, 1, 2}
            ,{3, 4, 5}},

            {{0, 1, 2}
            ,{3, 4, 5}
            ,{6, 7, 8}
            ,{9, 10, 11}}
        };

        vector<vector<int>> ans = {
            {},
            {0},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 3, 2},
            {0, 1, 2, 5, 4, 3},
            {0, 1, 2, 5, 8, 11, 10, 9, 6, 3, 4, 7}
        };

        util::Test(SpiralMatrixTraversal, v, ans);
    }
}