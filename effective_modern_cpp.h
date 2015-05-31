#pragma once

#include "util.h"

#include <string>
#include <unordered_map>

namespace EffectiveModernCpp {

    using namespace std;

    //===================================== Item 5: Prefer auto to explicit type declarations. ======================================

    template <typename T>
    class TD;

    void Item5() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis;

        unordered_map<string, int> hash;
        for (size_t i = 0; i < 1000; ++i) {
            uint32_t rnd = dis(gen);
            string key = to_string(rnd);
            hash[key] = rnd;
        }

        static const size_t Iters = 100000;

        {
            util::TTimerGuard t("    Auto var time: ");
            size_t sum = 0;
            for (size_t i = 0; i < Iters; ++i) {
                for (const auto & p : hash) {

                    sum += p.second;
                    // TD<decltype(p)> td;
                }
            }
        }

        {
            util::TTimerGuard t("Explicit var time: ");
            size_t sum = 0;
            for (size_t i = 0; i < Iters; ++i) {
                // Since pair<...> type is not exactly what hash iter refers to, 
                // there is a temporary object is constructed on each turn
                for (const pair<string, int> & p : hash) {
                    sum += p.second;
                    // TD<decltype(p)> td;
                }
            }
        }

    }

    void Run() {
        Item5();
    }
}