#pragma once

#include <vector>
#include <limits>

namespace CountBits {
    using namespace std;

    size_t CountSparse(uint32_t n) {
        size_t count = 0;
        while (n) {
            ++count;
            n &= n - 1;
        }
        return count;
    }

    static const size_t BitsInChar = 8;
    size_t CountDense(uint32_t n) {
        size_t count = BitsInChar * sizeof(n);
        n = ~n;
        while (n) {
            --count;
            n &= n - 1;
        }
        return count;
    }

    static size_t BitCount[numeric_limits<unsigned char>::max()] = {};
    void PrecomputeBits() {
        for (size_t i = 0; i <= numeric_limits<unsigned char>::max(); ++i) {
            BitCount[i] = CountSparse(i);
        }
    }

    size_t CountPrecomp(uint32_t n) {
        return BitCount[n & 0xff] + 
               BitCount[(n >> BitsInChar) & 0xff] +
               BitCount[(n >> 2 * BitsInChar) & 0xff] +
               BitCount[(n >> 3 * BitsInChar) & 0xff];
    }

    size_t CountFast(uint32_t n)
    {
        n = n - ((n >> 1) & 0x55555555);
        n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
        return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
    }

    bool Test(uint32_t n, size_t res, size_t ans) {
        if (res != ans)
            cout << "FAIL: n=" << n << " res=" << res << " ans=" << ans << endl;
        return res == ans;
    }

    class Timer {
        chrono::time_point<chrono::system_clock> Time = chrono::system_clock::now();
    public:
        void Start() {
            Time = chrono::system_clock::now();
        }

        size_t Elapsed() const {
            return chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now() - Time).count();
        }
    };

    void RunPerfTest() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis;

        static const size_t Iters = 100000000;
        static const size_t Nums = 100000000;
        vector<uint32_t> v;
        for (size_t i = 0; i < Nums; ++i)
            v.push_back(dis(gen));
        
        Timer t;

        t.Start();
        for (size_t i = 0; i < Iters; ++i) {
            for (auto n : v) {
                CountSparse(n);
            }
        }
        cout << "CountSparse  " << t.Elapsed() << endl;        

        t.Start();
        for (size_t i = 0; i < Iters; ++i) {
            for (auto n : v) {
                CountDense(n);
            }
        }
        cout << "CountDense   " << t.Elapsed() << endl;

        t.Start();
        for (size_t i = 0; i < Iters; ++i) {
            for (auto n : v) {
                CountPrecomp(n);
            }
        }
        cout << "CountPrecomp " << t.Elapsed() << endl;

        t.Start();
        for (size_t i = 0; i < Iters; ++i) {
            for (auto n : v) {
                CountFast(n);
            }
        }
        cout << "CountFast " << t.Elapsed() << endl;
    }

    void Run() {
        vector<uint32_t> nums = { 0, 1, 2, 3, 5, 15, 16, 0xFFFFFFFF, 0x1FFFFFFF };
        vector<size_t> ans = { 0, 1, 1, 2, 2, 4, 1, 32, 29 };

        PrecomputeBits();
        bool ok = true;
        for (size_t i = 0; i < nums.size(); ++i) {
            ok = ok && Test(nums[i], CountSparse(nums[i]), ans[i]);
            ok = ok && Test(nums[i], CountDense(nums[i]), ans[i]);
            ok = ok && Test(nums[i], CountPrecomp(nums[i]), ans[i]);
            ok = ok && Test(nums[i], CountFast(nums[i]), ans[i]);
        }
        if (ok)
            cout << "OK" << endl;

        RunPerfTest();
    }
    
}