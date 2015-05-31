/* [Bloomberg]
http://www.careercup.com/question?id=5687263784599552

Given a sorted array of integers, using the same array, shuffle the integers to have unique elements and return the index. 

Sample input : [3, 3, 4, 5, 5, 6, 7, 7, 7] 
Sample output : [3, 4, 5, 6, 7, X, X, X, X] 
In this case, it returns an index of 4. 
The elements in the array after that index is negligible (don't care what value it is).
*/

#pragma once
#include "util.h"
#include <vector>

namespace RemoveDups {
    using namespace std;

    size_t RemoveDups(vector<int> & v) {
        if (v.empty())
            return 0;

        int prev = v[0];
        int writeIdx = 1;
        for (size_t i = 1; i < v.size(); ++i) {
            if (v[i] == v[i - 1])
                continue;
            v[writeIdx++] = v[i];
        }
        return writeIdx - 1;
    }



    void Run() {
        vector<vector<int>> v = { { 3, 3, 4, 5, 5, 6, 7, 7, 7 }
                                , { }
                                , { 1 }
                                , { 1, 1 }
                                , { 1, 2, 2, 2 }
                                , { 1, 2, 3 }
                                , { 1, 2, 2, 3 }
                                , { 1, 1, 2, 2, 3, 3 }
                                };
        vector<size_t> ans = { 4
                             , 0
                             , 0
                             , 0
                             , 1
                             , 2
                             , 2
                             , 2
                             };

        util::Test(&RemoveDups, v, ans);
    }
}