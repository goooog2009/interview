#pragma once

#include "util.h"

#include <list>
#include <unordered_map>
#include <functional>

namespace LRUCache {

    using namespace std;

    template <typename Key, typename Val>
    class LRUCache {
        using TList = list<const Key>;
        using TListIt = typename TList::iterator;
        TList List;

        struct HashVal {
            Val Value;
            TListIt Iter;
        };
        unordered_map<Key, HashVal> Hash;
        size_t MaxSize;
    public:
        LRUCache(size_t maxSize)
            : MaxSize(max<size_t>(1, maxSize))
        {}

        // O(1)
        void Put(const Key & key, const Val & val) {
            auto i = Hash.find(key);

            // New element => add
            if (i == Hash.end()) {
                List.push_front(key);
                Hash.emplace(key, HashVal{val, List.begin()});

                // Discard if needed
                if (List.size() > MaxSize) {
                    Hash.erase(List.back());
                    List.pop_back();
                }
                return;
            }

            // Old element => renew
            i->second.Value = val;
            Promote(key, i->second.Iter);
        }

        // O(1)
        bool Get(const Key & key, Val & val) {
            auto i = Hash.find(key);

            if (i == Hash.end())
                return false;

            val = i->second.Value;
            Promote(key, i->second.Iter);
            return true;
        }

    private:
        void Promote(const Key & key, TListIt & iter) {
            List.erase(iter);
            List.push_front(key);
            iter = List.begin();
        }
    };

    void Run() {
        bool ok = true;

        const size_t CacheSize = 4;
        LRUCache<int, int> cache(CacheSize);

        for (int i = 1; i <= CacheSize; ++i) {
            cache.Put(i, i * 100);
        }

        int val = 0;
        bool found = false;

        for (int i = 1; i <= CacheSize; ++i) {
            found = cache.Get(i, val);
            ok = ok && util::TestOne(i, true, found);
            ok = ok && util::TestOne(i, val, i * 100);
        }

        for (int i = 1; i <= CacheSize; ++i) {
            cache.Put(i, i * 100 + 1);
        }

        for (int i = 1; i <= CacheSize; ++i) {
            found = cache.Get(i, val);
            ok = ok && util::TestOne(i, true, found);
            ok = ok && util::TestOne(i, val, i * 100 + 1);
        }

        {
            found = cache.Get(5, val);
            ok = ok && util::TestOne(5, false, found);

            cache.Put(5, 500);
            found = cache.Get(5, val);
            ok = ok && util::TestOne(5, true, found);
            ok = ok && util::TestOne(5, val, 500);

            // Oldest key should be discarded
            found = cache.Get(1, val);
            ok = ok && util::TestOne(1, false, found);
            
            // The rest of keys should still be there
            for (int i = 2; i <= CacheSize; ++i) {
                found = cache.Get(i, val);
                ok = ok && util::TestOne(i, true, found);
                ok = ok && util::TestOne(i, val, i * 100 + 1);
            }
        }

        {
            found = cache.Get(2, val); // Make 2 the LRU key

            // Rewrite all but one keys
            for (int i = 6; i <= 8; ++i) {
                cache.Put(i, i * 100);
            }

            // Key 2 should survive
            found = cache.Get(2, val);
            ok = ok && util::TestOne(2, true, found);
            ok = ok && util::TestOne(2, val, 201);
        }

        std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
    }
}