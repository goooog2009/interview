/* [Twitter] http://www.careercup.com/question?id=21249679
For a technical phone screen: 
Given a string "aaabbcccc", write a program to find the character with the second highest frequency.
*/

#pragma once

#include "util.h"

#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>

namespace FindChar {
    using namespace std;

    char Find2ndFreqChar(const string & str) {
        const size_t NthHighestFreq = 2;

        array<size_t, numeric_limits<char>::max() + 1> cnt = {};
        for (char c : str) {
            ++cnt[c];
        }

        using TCharCnt = tuple<size_t, char>;
        priority_queue<TCharCnt, vector<TCharCnt>, greater<TCharCnt>> q;
        for (char c = 0; c < cnt.size(); ++c) {
            if (!cnt[c])
                continue;

            q.push(make_tuple(cnt[c], c));
            if (q.size() > NthHighestFreq)
                q.pop();
        }

        return get<1>(q.top());
    }


    void Run() {
        vector<string> v = { "aaabbcccc", "bab", "ababababbggggggggg" };
        vector<char> ans = { 'a', 'a', 'b' };

        util::Test(Find2ndFreqChar, v, ans);

    }
}