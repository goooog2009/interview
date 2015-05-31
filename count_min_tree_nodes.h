/* [Amazon] http://www.careercup.com/question?id=4884741427822592

Assuming you have a binary tree which stores integers, count the number
of nodes whose value is lower than the value of its upper nodes.
*/

#pragma once

#include "util.h"
#include <memory>
#include <stack>

namespace NCountMinTreeNodes {
    using namespace std;

    struct Node;
    using NodePtr = unique_ptr<Node>;

    struct Node {
        int Value;
        NodePtr Left;
        NodePtr Right;

        Node(int val)
            : Node(val, nullptr, nullptr)
        {}

        Node(int val, NodePtr&& left, NodePtr&& right)
            : Value(val)
            , Left(move(left))
            , Right(move(right))
        {}
    };

    size_t CountMinTreeNodesImpl(const NodePtr& node, stack<int>& mins) {
        if (!node)
            return 0;

        size_t count = 0;
        if (node->Value < mins.top())
            ++count;

        mins.push(min(node->Value, mins.top()));

        count += CountMinTreeNodesImpl(node->Left, mins) + 
                 CountMinTreeNodesImpl(node->Right, mins);
        
        mins.pop();

        return count;
    }

    // O(n) time and memory
    size_t CountMinTreeNodes(const NodePtr& root) {
        if (!root)
            return 0;

        stack<int> mins;
        mins.push(root->Value);
        return 1 + // root element itself
               CountMinTreeNodesImpl(root->Left, mins) +
               CountMinTreeNodesImpl(root->Right, mins); 
    }
}

void TestCountMinTreeNodes() {
    using namespace NCountMinTreeNodes;
    using NCountMinTreeNodes::Node;

    NodePtr root = make_unique<Node>(5
                                    , make_unique<Node>(4
                                                        , make_unique<Node>(0)
                                                        , make_unique<Node>(5))
                                    , make_unique<Node>(6
                                                        , make_unique<Node>(4
                                                                            , nullptr
                                                                            , make_unique<Node>(10
                                                                                                , make_unique<Node>(7)
                                                                                                , nullptr))
                                                        , make_unique<Node>(7))
                                    );

    util::TestOne("case 0", CountMinTreeNodes(root), 4ul);
}