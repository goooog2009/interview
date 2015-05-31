/* [Pinterest]
http://www.glassdoor.com/Interview/Enumerate-all-NxN-grids-that-can-be-composed-of-valid-English-words-Architect-a-chess-program-And-an-amusing-question-abo-QTN_737998.htm

Enumerate all NxN grids that can be composed of valid English words.
*/

#pragma once

#include "util.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

namespace NxN_GridOfWords {
    using namespace std;

    bool CheckIfValid(const vector<string> & grid, const unordered_set<string> & words) {
        vector<string> vertWords(grid.size());
        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid.size(); ++j) {
                vertWords[j].push_back(grid[i][j]);
            }
        }

        for (const auto & w : vertWords) {
            if (words.find(w) == words.end())
                return false;
        }
        return true;
    }

    void PrintGrid(const vector<string> & grid) {
        for (const auto & w : grid) {
            cout << w << endl;
        }
        cout << endl;
    }

    void EnumerateImpl(size_t n, vector<string> & grid, const unordered_set<string> & words) {

        if (grid.size() == n) {
            if (CheckIfValid(grid, words))
                PrintGrid(grid);
            return;
        }

        for (const auto & w : words) {
            grid.push_back(w);
            EnumerateImpl(n, grid, words);
            grid.resize(grid.size() - 1);
        }
    }

    // Brute force algorithm
    // If k is size of dictionary and n is size of grid, then
    // complexity is O(k^n).
    void Enumerate(size_t n, const vector<string> & words) {
        unordered_set<string> wordsN;
        for (const auto & w : words) {
            if (w.size() == n)
                wordsN.emplace(w);
        }

        vector<string> grid;
        return EnumerateImpl(n, grid, wordsN);
    }

    vector<string> ReadWords(const string & filename) {
        vector<string> words;
        ifstream f(filename);
        string s;
        while (f >> s) {
            if (!s.empty())
                words.emplace_back(s);
        }
        return words;
    }

    void Run() {
        vector<string> words = ReadWords("/usr/share/dict/words");
        cout << "word count: " << words.size() << endl;
        
        Enumerate(3, words);
    }
}