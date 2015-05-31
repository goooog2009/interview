/*
Implement LFU cache.

TODO: There is also implementation with O(1) complexity. Implement it!
*/

#pragma once

#include "util.h"

#include <unordered_map>
#include <set>

// Works in O(log N)
namespace LFUCache {

    using namespace std;

    template <typename TKey, typename TVal>
    class LFUCache {
        size_t MaxSize;

        struct THashVal {
            TVal Value;
            size_t Counter = 0;

            THashVal(const TVal & val, size_t counter)
                : Value(val)
                , Counter(counter)
            {}
        };

        using THash = unordered_map<TKey, THashVal>;
        using THashIt = typename THash::iterator;
        THash Hash;

        struct TSetVal {
            TKey Key;
            size_t Counter = 0;

            TSetVal(const TKey & key, size_t counter)
                : Key(key)
                , Counter(counter)
            {}

            bool operator<(const TSetVal & rhs) const {
                return Counter < rhs.Counter || (Counter == rhs.Counter && Key < rhs.Key);
            }
        };

        using TSet = set<TSetVal>;
        using TSetIt = typename TSet::iterator;
        TSet Set;
    public:
        LFUCache(size_t maxSize)
            : MaxSize(max<size_t>(1, maxSize))
        {
        }

        // O(log N)
        void Put(const TKey & key, const TVal & val) {
            THashIt i = Hash.find(key);

            // Found item in cache, just update its value
            if (i != Hash.end()) {
                i->second.Value = val;
                return;
            }

            // Remove item with lowest freq., if needed
            if (Hash.size() >= MaxSize) {
                TSetIt j = Set.begin();
                Hash.erase(j->Key);
                Set.erase(j);
            }

            // Add new item to cache
            Hash.emplace(key, THashVal{val, 1});
            Set.emplace(key, 1);
        }

        // O(log N)
        bool Get(const TKey & key, TVal & val) {
            THashIt i = Hash.find(key);

            if (i == Hash.end())
                return false;

            // Increment counter
            Set.erase({key, i->second.Counter});
            ++i->second.Counter;
            Set.emplace(key, i->second.Counter);
            val = i->second.Value;

            return true;
        }
    };

    void Run() {
        bool ok = true;

        const size_t CacheSize = 5;
        LFUCache<int, int> cache(CacheSize);

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

            // Add one more key
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