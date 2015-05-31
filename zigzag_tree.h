/* [Amazon] http://www.careercup.com/question?id=5177676899811328

Write a function that accepts root of a binary tree and print zigzag level order traversal, each level print in new line. 

For example, 
Given tree: 
     1 
   2   3 
  4 5 6 7 
 8 9 

output: 
1 
2 3 
7 6 5 4 
8 9
*/

#pragma once

#include <string>
#include <vector>
#include <queue>
#include <memory>

namespace {

    using namespace std;

    struct Node {
        int Key;
        shared_ptr<Node> Left;
        shared_ptr<Node> Right;

        Node(int key)
            : Key(key)
        {}
    };

    shared_ptr<Node> DecodeTree(const vector<int>& tree) {
        if (tree.empty())
            return nullptr;

        queue<shared_ptr<Node>> nodes;

        shared_ptr<Node> root = make_shared<Node>(tree[0]);
        nodes.push(root);

        size_t peers = 2;
        size_t beg = 1;
        while (beg < tree.size()) {
            queue<shared_ptr<Node>> currNodes;

            for (size_t beg2 = beg; beg2 < min(tree.size(), beg + peers); beg2 += 2) {
                nodes.front()->Left = make_shared<Node>(tree[beg2]);
                currNodes.push(nodes.front()->Left);
                
                if (beg2 + 1 < tree.size()) {
                    nodes.front()->Right = make_shared<Node>(tree[beg2 + 1]);
                    currNodes.push(nodes.front()->Right);
                }

                nodes.pop();
            }

            nodes.swap(currNodes);

            beg += peers;
            peers *= 2;
        }

        return root;
    }

    string PrintTreeZigZag(const vector<int>& tree) {
        shared_ptr<Node> root = DecodeTree(tree);

        if (!root)
            return "";

        vector<shared_ptr<Node>> peers;
        peers.push_back(root);
        bool leftToRight = false;

        string res;

        while (!peers.empty()) {
            // Print peers we have
            if (leftToRight) {
                for (size_t i = 0; i < peers.size(); ++i) {
                    res += to_string(peers[i]->Key) + ' ';
                }
            } else {
                for (size_t i = peers.size(); i > 0; --i) {
                    res += to_string(peers[i - 1]->Key) + ' ';
                }
            }
            res += '\n';
            leftToRight = !leftToRight;

            // Read next level peers
            vector<shared_ptr<Node>> nextPeers;
            for (const auto& i : peers) {
                if (i->Left)
                    nextPeers.push_back(i->Left);
                if (i->Right)
                    nextPeers.push_back(i->Right);
            }
            peers.swap(nextPeers);
        }

        return res;
    }
}

void TestPrintTreeZigZag() {
    vector<vector<int>> v = { { 1, 
                                2, 3,
                                4, 5, 6, 7,
                                8, 9} };
    vector<string> ans = { "1 \n2 3 \n7 6 5 4 \n8 9 \n" };

    util::Test(PrintTreeZigZag, v, ans);
}