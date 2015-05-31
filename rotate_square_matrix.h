/* [Amazon] http://www.careercup.com/question?id=5121919756009472

Write code to rotate a square matrix: 
Input: 
1 2 3 
4 5 6 
7 8 9 

Output: 
4 1 2 
7 5 3 
8 9 6
*/

#pragma once

#include "util.h"

#include <vector>

namespace {
    void RotateSquareMatrix(vector<vector<int>>& v) {
        if (v.empty() ||
            v.size() != v[0].size())
        {
            throw runtime_error("bad vector");
        }

        if (v.size() == 1)
            return;

        size_t rows = v.size();
        size_t cols = v.front().size();

        for (size_t k = 0; k < v.size() / 2; ++k) {
            size_t maxRow = rows - k;
            size_t maxCol = cols - k;

            int tmp = v[k][k];

            for (size_t j = 1 + k; j < maxCol; ++j) {
                swap(tmp, v[k][j]);
            }

            for (size_t i = 1 + k; i < maxRow; ++i) {
                swap(tmp, v[i][cols - 1 - k]);
            }

            for (size_t j = maxCol - 1; j > k; --j) {
                swap(tmp, v[maxRow - 1][j - 1]);
            }

            for (size_t i = maxRow - 1; i > k; --i) {
                swap(tmp, v[i - 1][k]);
            }
        }
    }
}

void TestRotateSquareMatrix() {
    {
        vector<vector<int>> v = { {1} };
        vector<vector<int>> a = { {1} };
        RotateSquareMatrix(v);
        bool ok = ok || v == a;
        PrintTestResult(ok);
    }

    {
        vector<vector<int>> v = { {1, 2}
                                , {3, 4} };
        vector<vector<int>> a = { {3, 1}
                                , {4, 2} };
        RotateSquareMatrix(v);
        bool ok = ok || v == a;
        PrintTestResult(ok);
    }    

    {
        vector<vector<int>> v = { {1, 2, 3}
                                , {4, 5, 6}
                                , {7, 8, 9} };
        vector<vector<int>> a = { {4, 1, 2}
                                , {7, 5, 3}
                                , {8, 9, 6} };
        RotateSquareMatrix(v);
        bool ok = ok || v == a;
        PrintTestResult(ok);
    }
    
    {
        vector<vector<int>> v = { {1,  2,  3,  4,  5}
                                , {6,  7,  8,  9,  10}
                                , {11, 12, 13, 14, 15}
                                , {16, 17, 18, 19, 20}
                                , {21, 22, 23, 24, 25} };
        vector<vector<int>> a = { {6,  1,  2,  3,  4}
                                , {11, 12, 7,  8,  5}
                                , {16, 17, 13, 9,  10}
                                , {21, 18, 19, 14, 15}
                                , {22, 23, 24, 25, 20} };
        RotateSquareMatrix(v);
        bool ok = ok || v == a;
        PrintTestResult(ok);
    }

    
}