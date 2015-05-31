/* [Twitter] http://www.careercup.com/question?id=5768136489959424

write a function that has an int as input and return the equivalent String as an output 

12 -> 'twelve' 
4345 -> 'four thousand three hundred and forty-five' 
7654567643 -> 'seven billion six hundred and fifty-four million five hundred and sixty-seven thousand six hundred and forty-three'
*/

#pragma once
#include "util.h"
#include <vector>
#include <string>
#include <algorithm>

namespace SpellInt {
    using namespace std;

    string Spell(int n) {
        static const string digit[] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
        static const string teen[] = { "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };
        static const string ty[] = { "", "", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety" };
        static const string base[] = { "", " thousand", " million", " billion" };

        if (n == 0)
            return "zero";

        bool negative = n < 0;
        if (negative)
            n = -n;

        string res;
        size_t baseIdx = 0;
        do {
            size_t x = n % 1000;

            string s;

            if (x == 0) {
                // do nothing
            } else if (x < 10) {
                s = digit[x];
            } else if (x < 20) {
                s = teen[x - 10];
            } else if (x < 100) {
                s = ty[x / 10];
                if (x % 10) {
                    s += '-' + digit[x % 10];
                }
            } else { // x >= 100
                s = digit[x / 100] + " hundred" + (x % 100 > 0 ? " and " + Spell(x % 100) : "");
            }

            s += base[baseIdx++];
            res = s + (res.empty() ? "" : " ") + res;
            n /= 1000;
        } while (n);

        if (negative)
            res = "minus " + res;

        return res;
    }

    void Run() {
        vector<int> vec = { 12, 4345, -4126033, 0, 1, 5, 9, 10, 11, 19, 20, 35, 99, 100, 555, 1000 };
        vector<string> ans = { "twelve"
                             , "four thousand three hundred and fourty-five"
                             , "minus four million one hundred and twenty-six thousand thirty-three"
                             , "zero"
                             , "one"
                             , "five"
                             , "nine"
                             , "ten"
                             , "eleven"
                             , "nineteen"
                             , "twenty"
                             , "thirty-five"
                             , "ninety-nine"
                             , "one hundred"
                             , "five hundred and fifty-five"
                             , "one thousand" };

        util::Test(Spell, vec, ans);
    }
}