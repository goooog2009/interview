/* [Pinterest] http://www.careercup.com/question?id=5359122669109248

Given an input string and a dictionary of words, find out if the input string
can be segmented into a space-separated sequence of dictionary words. You need
to output the minimum number of words.

For example,
input: "aaaisaname" 
dict: ("a", "aaa", "is", "name") 
output: "aaa is a name" 
Wrong output: "a a a is a name"

############################### See also ###############################

http://thenoisychannel.com/2011/08/08/retiring-a-great-interview-problem

Given an input string and a dictionary of words,
segment the input string into a space-separated
sequence of dictionary words if possible. For
example, if the input string is "applepie" and
dictionary contains a standard set of English words,
then we would return the string "apple pie" as output.
*/

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "util.h"

namespace WordBreak {
    using namespace std;

    // Works in O(2^n), since it considers all possible segmentations.
    // Explanation: since potentially there can be a space after each character,
    // we have n positions, each can be 0 (no space needed) or 1,
    // that gives us 2^n possible combinations.
    string WordBreak(const string & str, const unordered_set<string> & dict) {
        if (dict.find(str) != dict.end())
            return str;

        for (size_t i = 1; i <= str.size(); ++i) {
            const string prefix = str.substr(0, i);
            if (dict.find(prefix) != dict.end()) {
                const string suffix = WordBreak(str.substr(i), dict);
                if (!suffix.empty()) {
                    return prefix + ' ' + suffix;
                }
            }
        }
        return "";
    }


    unordered_map<string, string> memo;

    // This implementation works in O(n^2) since we consider all combinations of
    // suffixes and prefixes, but each suffix is processed only once due to memorization.
    string WordBreakFast(const string & str, const unordered_set<string> & dict) {
        if (memo.find(str) != memo.end())
            return memo[str];

        if (dict.find(str) != dict.end())
            return str;

        for (size_t i = 1; i <= str.size(); ++i) {
            const string prefix = str.substr(0, i);
            if (dict.find(prefix) != dict.end()) {
                const string suffix = WordBreakFast(str.substr(i), dict);
                if (!suffix.empty()) {
                    // No need to memorize anything here since we already matched suffix at this point, job is done
                    return prefix + ' ' + suffix;
                }
            }
        }

        memo[str] = "";
        return "";
    }

    // This function returns minimum number of words [Pinterest]
    string WordBreakMin(const string & str, const unordered_set<string> & dict) {
        if (dict.find(str) != dict.end())
            return str;

        string res;
        for (size_t i = 1; i <= str.size(); ++i) {
            const string prefix = str.substr(0, i);
            if (dict.find(prefix) != dict.end()) {
                const string suffix = WordBreakMin(str.substr(i), dict);
                if (!suffix.empty()) {
                    string s = prefix + ' ' + suffix;
                    if (res.empty() || s.size() < res.size())
                        res = s;
                }
            }
        }
        return res;
    }

    void Run() {
        // Test WordBreak and WordBreakFast
        vector<string> v = { "appleisafruit"
                           , "apple" };
        vector<unordered_set<string>> dicts = { {"apple", "is", "a", "fruit"}
                                              , {"apple", "a"} };
        vector<string> ans = { "apple is a fruit"
                             , "apple" };

        bool ok = true;
        for (size_t i = 0; i < v.size(); ++i) {
            ok = ok && util::TestOne(v[i], WordBreak(v[i], dicts[i]), ans[i]);

            memo.clear();
            ok = ok && util::TestOne(v[i], WordBreakFast(v[i], dicts[i]), ans[i]);            
        }
        
        // Test WordBreakMin
        vector<string> v2 = { "aaaisaname"
                            , "aaaisaname" };
        vector<unordered_set<string>> dicts2 = { {"a", "aaa", "is", "name"}
                                               , {"a", "aaa", "aaisa", "is", "name"} };
        vector<string> ans2 = { "aaa is a name"
                              , "a aaisa name" };

        for (size_t i = 0; i < v.size(); ++i) {
            ok = ok && util::TestOne(v[i], WordBreakMin(v2[i], dicts2[i]), ans2[i]);
        }

        std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
    }
}