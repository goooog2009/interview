/* [Dropbox] http://www.careercup.com/question?id=5090679891951616

This is the screening question from Dropbox. I see it appear in other topics as the question in the interview.
However I did not find a complete solution so I put my discussion here (http://www.capacode.com/?p=447) for your reference. 

Question: Given a pattern and a string, check whether the string matches the pattern.
For example: pattern "aba" and the string is "redblackred" then it matches because 
"a" is translated to red and "b" is translated to "black". Note that for each 
character in the pattern, the translation is not empty and unique.
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace CheckStringPattern {

using namespace std;

// Complexity is O(N^k) (N is string length, k - pattern length)
bool CheckStringPatternImpl(const char * pStr, size_t n, vector<string> & pVal, const vector<size_t> & pattern, size_t pNum) {
    // cout << "CheckStringPatternImpl s='" << string(pStr, n) << "', pattern=" << pattern[pNum] << "," << pVal[pattern[pNum]] << endl;

    // Whether this is the first time we've met this item of the pattern
    bool defined = !pVal[pattern[pNum]].empty();
    
    // Stop recursion if this is the last item of pattern
    if (pNum == pattern.size() - 1) {
        string s(pStr, n);

        if (defined)
            return s == pVal[pattern[pNum]];

        bool found = find(begin(pVal), end(pVal), s) != end(pVal);
        return !found;
    }

    // If the pattern has been defined earlier, just try to match it explicitly, no cycle is required
    if (defined) {
        size_t pValSize = pVal[pattern[pNum]].size();
        if (string(pStr, min(n, pValSize)) != pVal[pattern[pNum]])
            return false;
        return CheckStringPatternImpl(pStr + pValSize, n - pValSize, pVal, pattern, pNum + 1);
    }

    // The pattern has not been define previously, need to check all possible combinations
    for (size_t i = 1; i <= n; ++i) {
        string pNew = string(pStr, i);
        bool found = find(begin(pVal), end(pVal), pNew) != end(pVal);

        // If value matches ather pattern item, this is not what will work
        if (found)
            continue;

        // Try to define the new pattern value and go on
        pVal[pattern[pNum]] = pNew;
        if (CheckStringPatternImpl(pStr + i, n - i, pVal, pattern, pNum + 1))
            return true;
    }

    // In nothing matched, need to clear the value that could be initialized here
    pVal[pattern[pNum]].clear();

    return false;
}


bool CheckStringPattern(const string & str, const string & pattern) {
    if (str.empty() || pattern.empty())
        return false;

    vector<int> m(256, -1);
    size_t num = 0;
    for (char c : pattern) {
        if (m[c] < 0)
            m[c] = num++;
    }

    vector<size_t> patternIds(pattern.size());
    for (size_t i = 0; i < pattern.size(); ++i) {
        patternIds[i] = m[pattern[i]];
    }

    const char * pStr = str.c_str();
    size_t n = str.size();
    vector<string> pVal(num);

    return CheckStringPatternImpl(pStr, n, pVal, patternIds, 0);
}

void CheckResult(const string & str, const string & pattern, bool res, bool ans) {
    if (res != ans)
        cout << "ERROR! str:'" << str << "' pattern:'" << pattern << "' res:" << res << " corr:" << ans << endl;
    else
        cout << "OK. str:'" << str << "' pattern:'" << pattern << "' res:" << res << endl;
}


void Run() {
    cout << "CheckStringPattern" << endl;

    vector<string> str = { "redblackred", "redblackred", "", "", "a", "a", "bus", "busbus", "azzz", "xxzzxx", "xxzxxz" };
    vector<string> pattern = { "aba", "abc", "", "a", "", "a", "z", "aa", "ba", "abba", "abab" };
    vector<bool>   ans = { true, true, false, false, false, true, true, true, true, true, true };

    for (size_t i = 0; i < str.size(); ++i) {
        CheckResult(str[i], pattern[i], CheckStringPattern(str[i], pattern[i]), ans[i]);
    }
}

}