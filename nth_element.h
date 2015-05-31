/*
http://www.cc.gatech.edu/~mihail/medianCMU.pdf
*/

#pragma once

#include "util.h"

#include <vector>
#include <algorithm>

namespace NthElement {

    using namespace std;

    // n is 0-based
    int* NthElementImpl(int* begin, int* end, size_t n) {
        // Algorithm:
        // 1. Split interval into groups of 5
        // 2. Find median for each group
        // 3. Find median of medians by doing recursion call
        // 4. Reorder elements around that median
        // 5. Check if this is n-th element, recurse if not

        if (end - begin == 1)
            return begin;

        // 1 and 2
        static const size_t GroupSize = 5;
        int* mediansEnd = begin;
        for (int* groupBegin = begin, *groupEnd = groupBegin + GroupSize;
             groupEnd <= end;
             groupBegin += GroupSize, groupEnd = groupBegin + GroupSize )
        {
            nth_element(groupBegin, groupBegin + GroupSize / 2, groupEnd);
            swap(*mediansEnd, *(groupBegin + GroupSize / 2));
            ++mediansEnd;
        }

        // Handle last group which can be less than GroupSize
        size_t lastGroupSize = (end - begin) % GroupSize;
        if (lastGroupSize) {
            nth_element(end - lastGroupSize, end - lastGroupSize + lastGroupSize / 2, end);
            swap(*mediansEnd, *(end - lastGroupSize + lastGroupSize / 2));
            ++mediansEnd;
        }

        // 3
        int* mediansBegin = begin;
        int* median = NthElementImpl(mediansBegin, mediansEnd, (mediansEnd - mediansBegin) / 2);
        swap(*median, *begin);
        median = begin;

        // 4        
        int* left = begin + 1;
        int* right = end - 1;
        while (left <= right) {
            while (left <= right && *left <= *median)
                ++left;
            while (left <= right && *right >= *median)
                --right;
            if (left < right)
                swap(*left, *right);
        }
        swap(*right, *median);
        median = right;

        // 5
        size_t k = median - begin;
        if (k == n)
            return median;

        if (k > n)
            return NthElementImpl(begin, median, n);
        return NthElementImpl(median + 1, end, n - k - 1);
    }
 
    // O(n) algorithm
    int NthElement(vector<int>& v, size_t n) {
        if (n >= v.size())
            throw runtime_error("NthElement: bad n param");

        return *NthElementImpl(&*v.begin(), &*v.begin() + v.size(), n);
    }

    void Run() {
        vector<int> v = { 4, 2, 5, 0, 1, 3, 7, 6, 8 };
        util::TestOne("test case 1", NthElement(v, 5), 5);
        util::TestOne("test case 2", NthElement(v, 4), 4);
        util::TestOne("test case 3", NthElement(v, 3), 3);
        util::TestOne("test case 4", NthElement(v, 0), 0);
        util::TestOne("test case 5", NthElement(v, 8), 8);

        vector<int> v0 = { 0 };
        util::TestOne("test case 6", NthElement(v0, 0), 0);

        static const size_t TestsCount = 1000;
        static const size_t TestVectorSize = 1000;
        bool ok = true;
        for (size_t i = 0; i < TestsCount; ++i) {
            vector<int> v1(TestVectorSize, 0);
            util::InitRandInt(v1, 0, 500);
            vector<int> v2 = v1;

            size_t n = util::GenRandInt<size_t>(0, v1.size() - 1);

            int res = NthElement(v1, n);

            nth_element(v2.begin(), v2.begin() + n, v2.end());
            int ans = v2[n];

            ok = ok && util::TestOne("batch test", res, ans, true);
        }
        std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
    }
}