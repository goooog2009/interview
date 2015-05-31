/* [Twitter] http://www.careercup.com/question?id=12908675

Given a string representing roman numeral, find and return its numeric value. e.g. XXIV = 24 and so on.
*/

#pragma once

#include <string>
#include <vector>

#include "util.h"

namespace RomanNumbers {

    using namespace std;

    size_t GetNum(char c) {
        switch (c) {
            case 'I': return 1;
            case 'V': return 5;
            case 'X': return 10;
            default:  return 0;
        }
    }

    size_t RomanToArabic(const string & roman) {
        size_t res = 0;
        size_t prevNum = 0;

        for (size_t i = 0; i < roman.size(); ++i) {
            size_t num = GetNum(roman[roman.size() - i - 1]);

            if (num >= prevNum) {
                res += num;
                prevNum = num;
            } else
                res -= num;
        }

        return res;
    }

    void Run() {
        vector<string> v = { "I", "VI", "IV", "XIX", "XXVI", "XXIV", "XXXIIV", "XVIII" };
        vector<size_t> n = { 1, 6, 4, 19, 26, 24, 33, 18 };

        util::Test(RomanToArabic, v, n);
    }
}