/* [Twitter] http://www.careercup.com/question?id=16425693

Given 4 points, whose x and y coordinates are both integers. They are all different.
Write a function to check if they form a square. Points can be given in any order.
*/

#pragma once

#include <algorithm>
#include <vector>

namespace CheckSquarePoints {
    using namespace std;

    struct Point {
        int X;
        int Y;

        Point(int x, int y)
            : X(x)
            , Y(y)
        {}

        bool operator<(const Point & rhs) const {
            return X < rhs.X || (X == rhs.X && Y < rhs.Y);
        }
    };

    size_t Distance(const Point & p1, const Point & p2) {
        return sqrt((p1.X - p2.X)*(p1.X - p2.X) + (p1.Y - p2.Y)*(p1.Y - p2.Y));
    }

    /* Supports all possible square orientations like:
          I)  .   .   II)  .    III)     .
                         .   .       .       
              .   .        .               .
                                       .
    */
    bool CheckSquare(const Point & p1, const Point & p2, const Point & p3, const Point & p4) {
        vector<Point> points = {p1, p2, p3, p4};
        sort(points.begin(), points.end());

        return Distance(points[0], points[1]) == Distance(points[0], points[2]) &&
               Distance(points[0], points[3]) == Distance(points[1], points[2]);
    }

    void Run() {
        bool ok = true;
        ok = ok && CheckSquare(Point(0, 0), Point(1, 1), Point(0, 1), Point(1, 0)) == true;
        ok = ok && CheckSquare(Point(0, 0), Point(1, 1), Point(0, 1), Point(1, 2)) == false;
        ok = ok && CheckSquare(Point(1, 2), Point(3, 2), Point(3, 4), Point(1, 4)) == true;
        ok = ok && CheckSquare(Point(2, 2), Point(2, 4), Point(1, 3), Point(3, 3)) == true;
        ok = ok && CheckSquare(Point(1, 1), Point(2, -2), Point(5, -1), Point(4, 2)) == true;
        ok = ok && CheckSquare(Point(1, 1), Point(2, -2), Point(5, -1), Point(4, -2)) == false;
        std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
    }
}