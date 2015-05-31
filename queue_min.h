/*
Implement queue that can return minimum value in O(1)
*/

#pragma once

#include "util.h"
#include <queue>
#include <list>
#include <cstdlib>

namespace QueueMin {
    using namespace std;

    template <typename T>
    class TQueueMin {
        struct TMinHolder {
            const T & Ref;
            size_t Count;

            TMinHolder(const T & t, size_t count)
                : Ref(t)
                , Count(count)
            {}
        };

        queue<T> Queue;
        list<TMinHolder> MinQueue;
    public:
        bool Empty() const {
            return Queue.empty();
        }

        void Push(const T & t) {
            if (Queue.empty()) {
                Queue.push(t);
                MinQueue.emplace_back(Queue.back(), 1);
                return;
            }

            Queue.push(t);

            if (t > MinQueue.back().Ref) {
                MinQueue.emplace_back(Queue.back(), 1);
            } else if (t == MinQueue.back().Ref) {
                ++MinQueue.back().Count;
            } else { // T < MinQueue.back().Ref
                size_t count = 1; // pushed element
                while (!MinQueue.empty() && MinQueue.back().Ref >= t) {
                    count += MinQueue.back().Count;
                    MinQueue.pop_back();
                }
                MinQueue.emplace_back(Queue.back(), count);
            }
        }

        void Pop() {
            if (Queue.empty())
                throw runtime_error("pop error: queue is empty");

            Queue.pop();

            --MinQueue.front().Count;
            if (!MinQueue.front().Count)
                MinQueue.pop_front();
        }

        T Min() const {
            if (Queue.empty())
                throw runtime_error("queue is empty");

            return MinQueue.front().Ref;
        }
    };

    template <typename T>
    T FindMinElement(queue<T> queue) {
        T min = queue.front();

        while (!queue.empty()) {
            min = std::min(min, queue.front());
            queue.pop();
        }

        return min;
    }

    void Run() {
        TQueueMin<int> queue;
        std::queue<int> queue2;
        srand(time(nullptr));

        static const size_t TestQueueSize = 100000;
        static const size_t PushProbPerc = 55;
        bool ok = true;
        for (size_t i = 0; ok && i < TestQueueSize; ++i) {
            bool push = queue.Empty() || rand() % 100 < PushProbPerc;

            if (push) {
                int x = rand();
                queue.Push(x);
                queue2.push(x);
            } else {
                queue.Pop();
                queue2.pop();
            }

            if (queue.Empty() != queue2.empty())
                cout << "FAIL: empty check failed" << endl;

            if (queue.Empty())
                continue;

            if (queue.Min() != FindMinElement(queue2)) {
                cout << "FAIL: res=" << queue.Min() << " ans=" << FindMinElement(queue2) << endl;
                ok = false;
            }
        }

        if (ok)
            cout << "SUCCESS" << endl;
    }
}