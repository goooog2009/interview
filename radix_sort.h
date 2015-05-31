/*[Yandex]

Implement sorting algorithm for uint32 that works faster than std::sort.
Your score will be the smallest number of elements your alg able to sort quicker (less is better).
*/

#pragma once

#include "util.h"

#include <iostream>
#include <limits>
#include <array>
#include <vector>
#include <random>

namespace RadixSort {

    using namespace std;
    using namespace util;

    template <typename T>
    void Print(const T & v) {
        for (auto i : v)
            cout << i << " ";
        cout << endl;
    }

    size_t GetByte(uint32_t i, size_t byte) {
        static const size_t BitsInByte = 8;
        static const uint32_t ByteMask = 0xff;

        return ByteMask & (i >> byte * BitsInByte);
    }

    void RadixSort(vector<uint32_t> & v) {
        vector<uint32_t> tmp(v.size());

        for (size_t byte = 0; byte < 4; ++byte) {
            array<size_t, numeric_limits<unsigned char>::max() + 2> buckets = {};

            for (size_t i = 0; i < v.size(); ++i) {
                size_t b = GetByte(v[i], byte);
                ++buckets[b + 1];
            }

            for (size_t i = 1; i < buckets.size(); ++i) {
                buckets[i] += buckets[i - 1];
            }

            for (size_t i = 0; i < v.size(); ++i) {
                size_t b = GetByte(v[i], byte);
                size_t pos = buckets[b]++;

                tmp[pos] = v[i];
            }

            v.swap(tmp);
        }
    }

    void Test(vector<uint32_t> & orig, vector<uint32_t> & res) {
        sort(orig.begin(), orig.end());

        if (orig == res)
            cout << "OK" << endl;
        else {
            cout << "FAIL" << endl;
            for (size_t i = 0; i < orig.size(); ++i) {
                if (orig[i] != res[i]) {
                    cout << i << ": " << orig[i] << " != " << res[i] << endl;
                }

            }
        }

    }

    bool SpeedTest(size_t vectorSize, size_t iters) {
        vector<uint32_t> v(vectorSize);

        size_t radixSort = 0;
        size_t stdSort = 0;
        TTimer t;
        for (size_t i = 0; i < iters; ++i) {
            util::InitRandInt(v);
            auto v2 = v;

            t.Start();
            RadixSort(v2);
            radixSort += t.Elapsed();

            t.Start();
            sort(v.begin(), v.end());
            stdSort += t.Elapsed();

            if (v != v2) {
                cout << "ERROR!" << endl;
                break;
            }
        }
        cout << "RadixSort " << radixSort << " nanosec" << endl;
        cout << "std::sort " << stdSort << " nanosec" << endl;

        return radixSort < stdSort;
    }

    void Run() {

        static const size_t VSize = 1000;
        vector<uint32_t> v(VSize);
        util::InitRandInt(v);

        auto v2 = v;
        RadixSort(v2);
        Test(v, v2);

        cout << "Testing performance" << endl;

        static const size_t Iters = 10000;
        size_t vSize = VSize;
        size_t dSize = vSize / 2;
        while (dSize) {
            bool radixWins = SpeedTest(vSize, Iters);

            if (radixWins)
                vSize -= dSize;
            else
                vSize += dSize;

            dSize /= 2;

            cout << vSize << " " << dSize << endl;
        }

        cout << "vector size: " << vSize << endl;
    }
}