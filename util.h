#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <tuple>
#include <random>

#define RunTest(testNamespace) \
    cout << "===> " << #testNamespace << endl; \
    testNamespace::Run(); \
    cout << "<=== " << #testNamespace << endl;

#define RunTestFn(testFn) \
    cout << "===> Start test " << #testFn << endl; \
    Test##testFn(); \
    cout << "<=== Finished test " << #testFn << endl;

void PrintTestResult(bool ok) {
    std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
}

namespace util {

    class TTimer {
        std::chrono::time_point<std::chrono::system_clock> Time = std::chrono::system_clock::now();
    public:
        void Start() {
            Time = std::chrono::system_clock::now();
        }

        size_t Elapsed() const {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - Time).count();
        }
    };

    class TTimerGuard {
        TTimer Timer;
        std::string Name;
    public:
        explicit TTimerGuard(const std::string & name = "")
            : Name(name)
        {}

        ~TTimerGuard() {
            if (!Name.empty())
                std::cout << Name << Timer.Elapsed() << std::endl;
        }
    };

    template <typename T>
    std::string ToString(const T & t) {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    std::string ToString(const std::tuple<int, int, int> & t) {
        std::stringstream ss;
        ss << "{" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << "}";
        return ss.str();
    }

    std::string ToString(bool b) {
        return b ? "true" : "false";
    }    

    template <typename T>
    std::string ToString(const std::vector<T> & v) {
        std::stringstream ss;
        ss << '{';
        for (size_t i = 0; i < v.size(); ++i) {
            if (i)
                ss << ", ";
            ss << ToString(v[i]);
        }
        return ss.str() + '}';
    }

    template <typename T>
    T GenRandInt(const T & min = 0, const T & max = std::numeric_limits<T>::max()) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<T> dis(min, max);

        return dis(gen);
    }

    template <typename T>
    T GenRandReal(const T & min = 0, const T & max = 1) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<T> dis(min, max);

        return dis(gen);
    }

    template <typename T>
    void InitRandInt(std::vector<T> & v, const T & min = 0, const T & max = std::numeric_limits<T>::max()) {
        for (auto & i : v)
            i = GenRandInt<T>(min, max);
    }

    template <typename Arg, typename TResult>
    bool TestOne(const Arg & arg, const TResult & res, const TResult & answer, bool silent = false) {
        if (res != answer) {
            if (!silent)
                std::cout << "FAIL: " << ToString(arg) << " result=" << ToString(res) << " answer=" << ToString(answer) << std::endl;
            return false;
        }
        if (!silent)
            std::cout << "OK: " << ToString(arg) << " result=" << ToString(res) << std::endl;
        return true;
    }


    template <typename Fn, typename Arg, typename TResult>
    void Test(Fn fn, std::vector<Arg> & testSet, const std::vector<TResult> & answers) {
        bool ok = true;
        for (size_t i = 0; i < testSet.size(); ++i) {
            TResult res = fn(testSet[i]);
            ok = ok && TestOne(testSet[i], res, answers[i]);
        }
        std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
    }
}