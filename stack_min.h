/* [Twitter] http://www.careercup.com/question?id=12934670

Design a modified stack that in addition to Push and Pop can also provide minimum element present in the stack via Min function.
*/

#pragma once

#include "util.h"
#include <cstdlib>
#include <stack>
#include <algorithm>
#include <iostream>

namespace StackMin {
    using namespace std;

    template <typename T>
    class TStackMin {
        struct TMinHolder {
            const T & MinRef;
            size_t Count;

            TMinHolder(const T & t)
                : MinRef(t)
                , Count(0)
            {}
        };

        stack<T> Stack;
        stack<TMinHolder> MinStack;
    public:
        bool Empty() const {
            return Stack.empty();
        }

        void Push(const T & t) {
            Stack.push(t);

            if (MinStack.empty() || Stack.top() < MinStack.top().MinRef)
                MinStack.push(TMinHolder(Stack.top()));

            ++MinStack.top().Count;
        }

        void Pop() {
            --MinStack.top().Count;
            if (!MinStack.top().Count)
                MinStack.pop();

            Stack.pop();
        }

        T & Top() {
            return Stack.top();
        }

        const T & Top() const {
            return Stack.top();
        }

        const T & Min() const {
            return MinStack.top().MinRef;
        }

    };

    template <typename T>
    T FindMinElement(stack<T> stack) {
        T min = stack.top();

        while (!stack.empty()) {
            min = std::min(min, stack.top());
            stack.pop();
        }

        return min;
    }

    void Run() {
        TStackMin<int> stack;
        std::stack<int> stack2;
        srand(time(nullptr));

        static const size_t TestStackSize = 100000;
        static const size_t PushProbPerc = 55;
        bool ok = true;
        for (size_t i = 0; ok && i < TestStackSize; ++i) {
            bool push = stack.Empty() || rand() % 100 < PushProbPerc;

            if (push) {
                int x = rand();
                stack.Push(x);
                stack2.push(x);
            } else {
                stack.Pop();
                stack2.pop();
            }

            if (stack.Empty() != stack2.empty())
                cout << "FAIL: empty check failed" << endl;

            if (!stack.Empty() && stack.Min() != FindMinElement(stack2)) {
                cout << "FAIL: res=" << stack.Min() << " ans=" << FindMinElement(stack2) << endl;
                ok = false;
            }
        }

        if (ok)
            cout << "SUCCESS" << endl;
    }
}