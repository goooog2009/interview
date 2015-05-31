/* [Pinterest]
http://www.glassdoor.com/Interview/Enumerate-all-NxN-grids-that-can-be-composed-of-valid-English-words-Architect-a-chess-program-And-an-amusing-question-abo-QTN_737998.htm

Write a function that returns n first happy numbers.


http://mathworld.wolfram.com/HappyNumber.html
Let the sum of the squares of the digits of a positive integer s_0 be represented by s_1.
In a similar way, let the sum of the squares of the digits of s_1 be represented by s_2, and so on.

Iterating this sum-of-squared-digits map always eventually reaches one of the 10 numbers 0, 1, 4, 16,
20, 37, 42, 58, 89, or 145 (OEIS A039943; Porges 1945).

If s_i=1 for some i>=1, then the original integer s_0 is said to be happy. For example, starting with
7 gives the sequence 7, 49, 97, 130, 10, 1, so 7 is a happy number.

The first few happy numbers are 1, 7, 10, 13, 19, 23, 28, 31, 32, 44, 49, 68, 70, 79, 82, 86, 91, 94, 97, 100, ... (OEIS A007770).
*/

#pragma once

#include "util.h"

#include <vector>
#include <unordered_set>

namespace HappyNumbers {
    using namespace std;

    size_t SumOfSquaresOfDigits(size_t k) {
        size_t sum = 0;
        while (k) {
            size_t digit = k % 10;
            sum += digit*digit;
            k /= 10;
        }
        return sum;
    }

    bool CheckHappiness(size_t k, unordered_set<size_t> & cache) {
        static const unordered_set<size_t> endpoints = { 0, 1, 4, 16, 20, 37, 42, 58, 89, 145 };

        if (cache.find(k) != cache.end())
            return true;
        
        size_t sum = k;
        vector<size_t> sequence;
        while (true) {
            sum = SumOfSquaresOfDigits(sum);
            sequence.push_back(sum);
            if (endpoints.find(sum) != endpoints.end())
                break;
        }

        if (sum != 1)
            return false;

        cache.insert(sequence.begin(), sequence.end());

        return true;
    }

    // Returns n first happy numbers
    vector<size_t> FindHappyNumbers(size_t n) {
        vector<size_t> happyNumbers;
        unordered_set<size_t> cache;

        for (size_t k = 1; happyNumbers.size() < n; ++k) {
            if (!CheckHappiness(k, cache))
                continue;
            
            // k is happy :)
            happyNumbers.push_back(k);
        }

        return happyNumbers;
    }

    void Run() {
        vector<size_t> v = { 3
                           , 10
                           , 20 };
        vector<vector<size_t>> ans = { { 1, 7, 10 }
                                     , { 1, 7, 10, 13, 19, 23, 28, 31, 32, 44 }
                                     , { 1, 7, 10, 13, 19, 23, 28, 31, 32, 44, 49, 68, 70, 79, 82, 86, 91, 94, 97, 100 } };

        util::Test(FindHappyNumbers, v, ans);
    }
}