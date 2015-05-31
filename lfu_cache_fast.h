#pragma once

#include "util.h"

#include <unordered_map>
#include <list>
#include <algorithm>

namespace LFUCacheFast {
    using namespace std;

    // Works in O(1)
    template <typename TKey, typename TVal>
    class LFUCacheFast {
        struct TCounterInfo;
        using TCounterList = list<TCounterInfo>;
        using TCounterListIt = typename TCounterList::iterator;

        struct TKeyInfo {
            TKey Key;
            TCounterListIt Iter;

            TKeyInfo(const TKey & key, const TCounterListIt & it)
                : Key(key)
                , Iter(it)
            {}
        };
        using TKeyList = list<TKeyInfo>;
        using TKeyListIt = typename TKeyList::iterator;

        struct TCounterInfo {
            size_t Counter;
            TKeyList KeyList;

            TCounterInfo(size_t counter)
                : Counter(counter)
            {}
        };

        TCounterList CounterList;

        struct THashVal {
            TVal Value;
            TKeyListIt Iter;

            THashVal(const TVal & val, const TKeyListIt & it)
                : Value(val)
                , Iter(it)
            {}
        };

        using THash = unordered_map<TKey, THashVal>;
        using THashIt = typename THash::iterator;
        THash Hash;

        size_t MaxSize;
    public:
        LFUCacheFast(size_t maxSize)
            : MaxSize(max<size_t>(1, maxSize))
        {}

        void Put(const TKey & key, const TVal & val) {
            THashIt i = Hash.find(key);

            // Item has been already cached, just update its value
            if (i != Hash.end()) {
                i->second.Value = val;
                return;
            }

            // Check cache size and discard LFU iten if required
            if (Hash.size() >= MaxSize) {
                TCounterListIt cntIt = CounterList.begin();
                TKeyList & keyList = cntIt->KeyList;
                Hash.erase(keyList.begin()->Key);
                keyList.erase(keyList.begin());

                if (keyList.empty()) {
                    CounterList.erase(cntIt);
                }
            }

            // New item, need to add
            {
                if (CounterList.empty() || CounterList.front().Counter > 1)
                    CounterList.push_front(TCounterInfo(1));
                TCounterListIt cntIt = CounterList.begin();
                
                CounterList.front().KeyList.emplace_front(key, cntIt);
                TKeyListIt keyIt = CounterList.front().KeyList.begin();
                
                Hash.insert(make_pair(key, THashVal(val, keyIt)));
            }
        }

        bool Get(const TKey & key, TVal & val) {
            THashIt i = Hash.find(key);

            if (i == Hash.end())
                return false;

            // Increment counter

            TKeyListIt keyIt = i->second.Iter;

            TCounterListIt cntIt = keyIt->Iter;
            TCounterListIt nextCntIt = next(cntIt);

            if (nextCntIt->Counter == cntIt->Counter + 1) {
                nextCntIt->KeyList.emplace_front(key, nextCntIt);
                i->second.Iter = nextCntIt->KeyList.begin();
            } else {
                TCounterListIt newCntIt = CounterList.insert(nextCntIt, TCounterInfo(cntIt->Counter + 1));
                newCntIt->KeyList.emplace_front(key, newCntIt);
                i->second.Iter = newCntIt->KeyList.begin();
            }

            // Delete key from current counter bucket
            cntIt->KeyList.erase(keyIt);
            if (cntIt->KeyList.empty()) {
                CounterList.erase(cntIt);
            }

            val = i->second.Value;
            return true;
        }
    };

    void Run() {
        bool ok = true;

        const size_t CacheSize = 5;
        LFUCacheFast<int, int> cache(CacheSize);

        bool found = false;
        int val = -1;

        { // Just save and retrieve
            for (int i = 0; i < CacheSize; ++i) {
                cache.Put(i, i + 100);
            }

            for (int i = 0; i < CacheSize; ++i) {
                found = cache.Get(i, val);
                ok = ok && util::TestOne(i, found, true);
                ok = ok && util::TestOne(i, val, i + 100);
            }
        }

        { // Update values
            for (int i = 0; i < CacheSize; ++i) {
                cache.Put(i, i + 1000);
            }

            for (int i = 0; i < CacheSize; ++i) {
                found = cache.Get(i, val);
                ok = ok && util::TestOne(i, found, true);
                ok = ok && util::TestOne(i, val, i + 1000);
            }
        }

        { // Check eviction
            // Touch all but one element
            for (int i = 1; i < CacheSize; ++i) {
                cache.Get(i, val);
            }

            // Add one more key & touch it
            int newKey = CacheSize;
            cache.Put(newKey, newKey + 1000);

            // Check if least frequent element has gone
            found = cache.Get(0, val);
            ok = ok && util::TestOne(0, found, false);

            // Check that all other elements are still available
            for (int i = 1; i < CacheSize + 1; ++i) {
                found = cache.Get(i, val);
                ok = ok && util::TestOne(i, found, true);
                ok = ok && util::TestOne(i, val, i + 1000);
            }
        }

        std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
    }
}