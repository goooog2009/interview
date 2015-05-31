/* [Google] http://www.careercup.com/question?id=6303093824159744

If a canoe can hold 2 kids and a max weight of 150 lbs, write a function that returns
the minimum number of canoes needed, given a list of kids and their weights.
*/

#pragma once

#include <vector>
#include <algorithm>

namespace {
    size_t CanoeForKids(vector<size_t>& v) {
        static const size_t MaxLbs = 150;

        if (v.empty())
            return 0;

        sort(v.begin(), v.end(), greater<size_t>());
        
        size_t res = 0;
        
        size_t l = 0;
        size_t r = v.size() - 1;

        while (l <= r) {
            if (v[l] + v[r] <= MaxLbs) {
                ++res;
                ++l;
                --r;
            } else if (v[l] <= MaxLbs) {
                ++res;
                ++l;
            } else
                ++l; // Too heavy child to go, leave him
        }

        return res;
    }
}

void TestCanoeForKids() {
    vector<vector<size_t>> v = { {150, 140, 100, 80, 80, 75, 70, 50, 40, 40, 20, 10}
                               , {160, 150, 140, 10}
                               , {} };
    vector<size_t> ans = { 7
                         , 2
                         , 0 };

    util::Test(CanoeForKids, v, ans);
}