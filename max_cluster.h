/* [Amazon] http://www.careercup.com/question?id=5098159661383680

{1, 1, 0, 0, 0}, 
{1, 1, 0, 0, 0}, 
{1, 0, 0, 1, 1}, 
{0, 0, 0, 0, 0}, 
{1, 0, 1, 0, 1} 

Write a function to return the size of max cluster and coordinate of it, max cluster size to the above example is 5.
*/

#pragma once

#include "util.h"

#include <vector>

namespace {

    size_t ClusterSize(const vector<vector<int>>& v, vector<vector<bool>>& visited, int i, int j) {
        if (i < 0 || i >= v.size() || j < 0 || j >= v[0].size())
            return 0;

        if (visited[i][j])
            return 0;

        visited[i][j] = true;

        if (!v[i][j])
            return 0;

        // Found new cluster
        size_t clusterSize = 1;

        for (int di = -1; di <= 1; ++di) {
            for (int dj = -1; dj <= 1; ++ dj) {
                clusterSize += ClusterSize(v, visited, i + di, j + dj);
            }
        }

        return clusterSize;
    }

    size_t FindMaxCluster(const vector<vector<int>>& v, size_t& row, size_t& col) {
        if (v.empty())
            return 0;

        // Assume all rows are of same size
        vector<vector<bool>> visited(v.size(), vector<bool>(v[0].size(), false));

        size_t maxSize = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            for (size_t j = 0; j < v[i].size(); ++j) {
                size_t size = ClusterSize(v, visited, i, j);

                if (size > maxSize) {
                    maxSize = size;
                    row = i;
                    col = j;
                }
            }
        }

        return maxSize;
    }

}

void TestFindMaxCluster() {
    {
        vector<vector<int>> v = { {1, 1, 0, 0, 0}
                                , {1, 1, 0, 0, 0}
                                , {1, 0, 0, 1, 1}
                                , {0, 0, 0, 0, 0}
                                , {1, 0, 1, 0, 1} };

        size_t row = 0;
        size_t col = 0;
        util::TestOne("case 0 size", FindMaxCluster(v, row, col), 5ul);
        util::TestOne("case 0 row", row, 0ul);
        util::TestOne("case 0 col", col, 0ul);
    }

    {
        vector<vector<int>> v = { {1, 1, 0, 0, 0}
                                , {0, 0, 0, 0, 0}
                                , {1, 0, 1, 1, 1}
                                , {0, 0, 1, 0, 0}
                                , {1, 0, 1, 1, 1} };

        size_t row = 0;
        size_t col = 0;
        util::TestOne("case 1 size", FindMaxCluster(v, row, col), 7ul);
        util::TestOne("case 1 row", row, 2ul);
        util::TestOne("case 1 col", col, 2ul);
    }
}