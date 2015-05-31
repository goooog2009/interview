/* [Twitter] http://www.careercup.com/question?id=6198772753235968

Given a list of names. Find whether a particular name occurs inside a given tweet or not.
If found return true otherwise false Time complexity should be less than O(n). 

Ex: "Katy Perry","Ronan Keating" given as a list of string. 

List<String> names; 
bool findName(String tweet) 
{ 

}
*/
#pragma once

#include "util.h"

#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <unordered_set>

namespace FindNameInTweet {
    using namespace std;

    static vector<string> Names = {"Bob", "Bob Smith", "Bob Marley", "Jack", "Jack Stone", "Martin Luther King", "Martin"};

    vector<string> Tokenize(const string & str) {
        vector<string> tokens;

        if (str.empty())
            return tokens;

        string s = str + ' ';
        enum State { Separator, Token };

        State prevState = Separator;
        size_t beg = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            State state = isalnum(s[i]) ? Token : Separator;

            if (prevState == Separator && state == Token) {
                beg = i;
            } else if (prevState == Token && state == Separator) {
                if (i > beg)
                    tokens.emplace_back(s.begin() + beg, s.begin() + i);
            }

            prevState = state;
        }
        return tokens;
    }

    class TNameMatcher {
        unordered_set<string> Names;
        size_t MaxNameToks = 0;

        static const char SEP = ' ';
    public:
        TNameMatcher(const vector<string> & names) {
            for (const auto & name : names) {
                vector<string> toks = Tokenize(name);
                MaxNameToks = max(toks.size(), MaxNameToks);

                string s;
                for (const auto & t : toks) {
                    if (!s.empty())
                        s += SEP;
                    s += t;
                }
                Names.insert(s);
            }
        }

        string FindMatch(const string & s) const {
            vector<string> toks = Tokenize(s);

            string res;
            size_t longestMatch = 0;

            size_t i = 0;
            while (i < toks.size()) {
                for (size_t j = 1; j <= min(MaxNameToks, toks.size() - i); ++j) {
                    string name;
                    for (size_t k = i; k < i + j; ++k) {
                        if (!name.empty())
                            name += SEP;
                        name += toks[k];
                    }
                    if (Names.count(name) && j > longestMatch) {
                        res = name;
                        longestMatch = j;
                    }
                }
                ++i;
            }

            return res;
        }
    };

    string FindName(const string & tweet) {
        static TNameMatcher matcher(Names);
        string name = matcher.FindMatch(tweet);
        return name;
    }

    void Run() {
        vector<string> tweets = { "hi Bob!"
                                , "Bob Marley is a musician"
                                , "Bob and Jack"
                                , "Martin Luther King hi there"
                                , "Hi Bob!!! I like Martin Luther King"
                                };
        vector<string> ans { "Bob"
                           , "Bob Marley"
                           , "Bob"
                           , "Martin Luther King"
                           , "Martin Luther King"
                           };
        util::Test(FindName, tweets, ans);
    }
}