/* [Amazon] http://www.careercup.com/question?id=5630442004807680

given an array with elements check if just by exchanging two elements of the array we get a sorted array

time restriction: 
    O(NlogN)
    space restriction: 2N

Note: this can be done in O(n) actually using const memory.
*/

#pragma once

#include "util.h"

#include <vector>
#include <deque>
#include <algorithm>

namespace {
    using namespace std;

    // O(n log n) solution
    bool CheckSwapToSort(const vector<int>& v) {
        if (v.size() < 2)
            return false;

        vector<int> v2 = v;
        sort(v2.begin(), v2.end());

        size_t diff = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i] != v2[i])
                ++diff;
        }

        return diff == 2;
    }

    // O(n) solution using const memory (if input array can be modified)
    bool CheckSwapToSort2(const vector<int>& m) {
        if (m.size() < 2)
            return false;

        vector<int> v(m); // Copying input vector for unit test simplicity only

        int first = -1;
        int second = -1;
        for (size_t i = 1; i < v.size(); ++i) {
            if (v[i - 1] <= v[i])
                continue;

            // Found misordered pair

            if (first == -1)
                first = i - 1;

            second = i;

            // Advance second to the rightmost element in the sequence of equals (if any)
            while (second + 1 < v.size() && v[second + 1] == v[second])
                ++second;
        }

        // At this point first holds index of first misordered element
        // and second -- index of the last one.

        if (first == -1)
            return false;

        swap(v[first], v[second]);
        for (size_t i = 1; i < v.size(); ++i) {
            if (v[i - 1] > v[i])
                return false;
        }

        return true;
    }

    // This implementation:
    //   * works in O(n)
    //   * uses const memory
    //   * doesn't modify input array
    bool CheckSwapToSort3(const vector<int>& v) {
        if (v.size() < 2)
            return false;

        int first = -1;
        int second = -1;
        for (size_t i = 1; i < v.size(); ++i) {
            if (v[i - 1] <= v[i])
                continue;

            if (first == -1) {
                first = i - 1;
            }

            second = i;

            while (second + 1 < v.size() && v[second + 1] == v[second])
                ++second;
        }

        if (first == -1)
            return false;

        // Now we won't really swap elements in v,
        // but will just check if swapping would make it sorted

        if (v[second] > v[first + 1] || v[first] < v[second - 1])
            return false;

        for (size_t i = first + 2; i <= second - 1; ++i) {
            if (v[i - 1] > v[i])
                return false;
        }

        return true;
    }
}

void TestCheckSwapToSort() {
    vector<vector<int>> v = { {1, 2, 3, 4, 5}
                            , {1, 3, 2, 4, 5}
                            , {5, 2, 3, 4, 1}
                            , {5, 4, 3, 2, 1}
                            , {1, 2, 3, 5, 4}
                            , {0, 2, 1, 0, 3, 4, 5}
                            , {}
                            , {0}
                            , {1, 2}
                            , {2, 1}
                            , {0, 0}
                            , {4, 2, 2, 2, 5}
                            , {3, 2, 2, 2, 3, 3, 4, 4, 4}
                            , {3, 2, 2}
                            , {3, 2, 4} };

    // Workaround for vector<bool>
    vector<int> ans = { false
                      , true
                      , true
                      , false
                      , true
                      , true
                      , false
                      , false
                      , false
                      , true
                      , false
                      , true
                      , true
                      , true
                      , true };

    util::Test(CheckSwapToSort, v, ans);
    util::Test(CheckSwapToSort2, v, ans);
    util::Test(CheckSwapToSort3, v, ans);
}