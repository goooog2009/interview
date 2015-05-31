/* [LinkedIn] http://leetcode.com/2010/11/finding-minimum-window-in-s-which.html

Find the Minimum Window in S which Contains All Elements from T.
T may contain duplicate items.
*/

#pragma once

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

namespace MinWindow {

using namespace std;

typedef bool (*FnPtr)(const string &, const string &, size_t &, size_t &);

// Works with complexity O(N), where N is the length of S
bool FindMinWindow(const string & str, const string & pattern, size_t & begin, size_t & end) {
    unordered_map<char, size_t> patternItemsCount; // How many pattern items we need to match (we need counters since there might be duplicates in T)
    for (const char & c : pattern)
        ++patternItemsCount[c];

    list<size_t> matchedIdx; // Global list of indicies of matched items
    unordered_map<char, list< list<size_t>::iterator >> matchedIdxByItem; // List of iterators to the indicies of matched items

    size_t minWindowSize = str.size() + 1;
    size_t bestBegin = 0;
    size_t bestEnd = 0;
    size_t matched = 0;
    for (size_t i = 0; i < str.size(); ++i) { // <- O(N)
        char c = str[i];

        // Check if the item is in the pattern
        if (patternItemsCount.find(c) == patternItemsCount.end())
            continue;

        // Add pattern item index to the queue
        ++matched;
        matchedIdx.push_back(i);
        list<size_t>::iterator it = matchedIdx.end();
        --it;
        matchedIdxByItem[c].push_back(it);

        // Drop previous indicies
        if (matchedIdxByItem[c].size() > patternItemsCount[c]) {
            --matched;
            matchedIdx.erase(matchedIdxByItem[c].front());
            matchedIdxByItem[c].pop_front();
        }

        // Check for match and minimum window
        size_t minIdx = *matchedIdx.begin();
        size_t maxIdx = *matchedIdx.rbegin();
        size_t windowSize = maxIdx - minIdx;
        if (matched == pattern.size() && windowSize < minWindowSize) {
            minWindowSize = windowSize;
            bestBegin = minIdx;
            bestEnd = maxIdx;
        }
    }

    if (minWindowSize > str.size())
        return false;

    begin = bestBegin;
    end = bestEnd;
    return true;
}

// Easier solution
bool FindMinWindow2(const string & str, const string & pattern, size_t & begin, size_t & end) {
    unordered_map<char, size_t> needToMatch;
    for (const auto & c : pattern)
        ++needToMatch[c];

    unordered_map<char, size_t> matched;
    size_t matchCount = 0;
    size_t matchBegin = 0;

    size_t bestBegin = 0;
    size_t bestEnd = str.size() + 1;
    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];

        // Skip if the current item is not in the pattern
        if (needToMatch.find(c) == needToMatch.end())
            continue;

        if (matched[c] < needToMatch[c])
            ++matchCount;
        ++matched[c];

        // Skip if we have not met the constraint
        if (matchCount != pattern.size())
            continue;

        // Move begin idx while constraint is met
        size_t newBeg = matchBegin;
        while (true) {
            if (needToMatch.find(str[newBeg]) == needToMatch.end()) {
                ++newBeg;
                continue;
            }

            if (matched[str[newBeg]] <= needToMatch[str[newBeg]])
                break;

            --matched[str[newBeg]];
            ++newBeg;
        }
        matchBegin = newBeg;

        if (i - matchBegin < bestEnd - bestBegin) {
            bestBegin = matchBegin;
            bestEnd = i;
        }
    }

    begin = bestBegin;
    end = bestEnd;
    return bestEnd - bestBegin <= str.size();
}

void CheckResult(const string & str, const string & pattern, size_t begin, size_t end, FnPtr func) {
    size_t b = 0;
    size_t e = 0;
    func(str, pattern, b, e);

    cout << "str: " << str << ", pattern: " << pattern << ", res: " << b << ", " << e;
    if (b != begin || e != end)
        cout << " <- FAIL! Correct res: " << begin << ", " << end;
    else
        cout << " <- OK";
    cout << endl;
}

void CheckResults(const string & str, const string & pattern, size_t begin, size_t end) {
    CheckResult(str, pattern, begin, end, &FindMinWindow);
    CheckResult(str, pattern, begin, end, &FindMinWindow2);
}

void Run() {
    cout << "MinWindow" << endl;
    CheckResults("ABNXCNBA", "ABC", 4, 7);
    CheckResults("ADOBECODEBANC", "ABC", 9, 12);
    CheckResults("acbbaca", "aba", 3, 6);
}

} // namespace MinWindow