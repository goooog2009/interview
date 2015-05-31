#pragma once

#include "util.h"

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

namespace KDTree {
    using namespace std;

    template <size_t Dimensions>
    class TPoint {
    public:
        using TCoord = array<float, Dimensions>;

        TPoint(const TCoord & coord, const string & name)
            : Coord(coord)
            , Name(name)
        {}

        TPoint(const TCoord & coord)
            : TPoint(coord, "")
        {}

        TPoint(const TPoint & p)
            : Coord(p.Coord)
            , Name(p.Name)
        {}

        float GetCoord(size_t i) const {
            return Coord[i];
        }

        constexpr static size_t Size() {
            return Dimensions;
        }

        bool operator==(const TPoint & rhs) const {
            return Coord == rhs.Coord && Name == rhs.Name;
        }

        // Return squared distance between two points
        float DistanceTo(const TPoint & rhs) const {
            float dist = 0;
            for (size_t i = 0; i < Coord.size(); ++i)
                dist += (GetCoord(i) - rhs.GetCoord(i)) * (GetCoord(i) - rhs.GetCoord(i));
            return dist;
        }

    private:
        TCoord Coord;
        string Name;
    };

    using TPoint2D = TPoint<2>;

    template <typename T>
    class KDTree {
        struct TNode;
        using TNodePtr = unique_ptr<TNode>;

        struct TNode {
            T Point;
            TNodePtr Left;
            TNodePtr Right;

            TNode(const T & t)
                : Point(t)
            {}
        };
        

    public:
        KDTree()
        {}

        KDTree(const T & t)
            : Root(make_unique<TNode>(t))
        {}

        KDTree(vector<T> & v) {
            Root = BuildTree(v.begin(), v.end());
        }

        bool Find(const T & t) const {
            return FindImpl(Root, t);
        }

        const T * FindNearest(const T & t, float & distance) const {
            const T * nearest = nullptr;
            
            FindNearestImpl(Root, t, nearest, distance);
            return nearest;
        }

        void Insert(const T & t) {
            InsertImpl(Root, t);
        }

    private:
        template <typename TIter>
        TNodePtr BuildTree(TIter begin, TIter end, size_t dim = 0) {
            if (begin >= end)
                return nullptr;

            TIter med = begin + (end - begin) / 2;
            nth_element(begin, med, end, [dim](const T & lhs, const T & rhs) { return lhs.GetCoord(dim) < rhs.GetCoord(dim); } );
            
            TNodePtr n(make_unique<TNode>(*med));

            size_t d = (dim + 1) % T::Size();
            n->Left = BuildTree(begin, med, d);
            n->Right = BuildTree(med + 1, end, d);

            return n;
        }

        void InsertImpl(TNodePtr & n, const T & t, size_t dim = 0) {
            if (!n) {
                n = make_unique<TNode>(t);
                return;
            }

            size_t d = (dim + 1) % T::Size();
            if (t.GetCoord(dim) < n->Point.GetCoord(dim))
                InsertImpl(n->Left, t, d);
            else
                InsertImpl(n->Right, t, d);
        }

        bool FindImpl(const TNodePtr & n, const T & t, size_t dim = 0) const {
            if (!n)
                return false;

            if (n->Point == t)
                return true;

            size_t d = (dim + 1) % T::Size();
            if (t.GetCoord(dim) < n->Point.GetCoord(dim))
                return FindImpl(n->Left, t, d);
            return FindImpl(n->Right, t, d);
        }

        void FindNearestImpl(const TNodePtr & n, const T & t, const T * & nearest, float & distance, size_t dim = 0) const {
            if (!n)
                return;

            float dist = t.DistanceTo(n->Point);
            if (!nearest || dist < distance) {
                nearest = &n->Point;
                distance = dist;
            }

            size_t d = (dim + 1) % T::Size();

            float diff = t.GetCoord(dim) - n->Point.GetCoord(dim);
            FindNearestImpl(diff < 0 ? n->Left : n->Right, t, nearest, distance, d);

            if (diff * diff >= distance)
                return;

            FindNearestImpl(diff < 0 ? n->Right : n->Left, t, nearest, distance, d);
        }

    private:
        TNodePtr Root;
    };

    const TPoint2D * FindNearest(const TPoint2D & point, const vector<TPoint2D> & v, float & dist) {
        const TPoint2D * res = nullptr;

        for (const auto & i : v) {
            float d = point.DistanceTo(i);
            if (!res || d < dist) {
                res = &i;
                dist = d;
            }
        }

        return res;
    }


    void Run() {
        bool ok = true;

        {
            vector<TPoint2D> v = { TPoint2D({1, 2})
                                 , TPoint2D({2, 1})
                                 , TPoint2D({3, 4}) };
            KDTree<TPoint2D> kdtree(v);
            kdtree.Insert(TPoint2D({4, 4}));

            

            ok = ok && util::TestOne("find {2, 1}", kdtree.Find(TPoint2D({2.0f, 1.0f})), true);
            ok = ok && util::TestOne("find {4, 4}", kdtree.Find(TPoint2D({2.0f, 1.0f})), true);

            float dist = 0;
            const TPoint2D * p = kdtree.FindNearest(TPoint2D({3, 2}), dist);
            ok = ok && util::TestOne("find nearest to {3, 2}", p->GetCoord(0), 2.0f);
            ok = ok && util::TestOne("find nearest to {3, 2}", p->GetCoord(1), 1.0f);
        }

        {
            vector<TPoint2D> v;
            static const size_t TreeSize = 100000;
            for (size_t i = 0; i < TreeSize; ++i) {
                TPoint2D p({util::GenRandReal<float>(), util::GenRandReal<float>()});
                v.push_back(p);
            }

            vector<TPoint2D> v2(v);
            KDTree<TPoint2D> kdtree(v2);

            static const size_t TestCases = 1000;
            for (size_t i = 0; i < TestCases; ++i) {
                TPoint2D p({util::GenRandReal<float>(), util::GenRandReal<float>()});

                float dist1 = 0;
                const TPoint2D * p1 = kdtree.FindNearest(p, dist1);

                float dist2 = 0;
                const TPoint2D * p2 = FindNearest(p, v, dist2);

                if (!(util::TestOne("find nearest batch", p1->GetCoord(0) == p2->GetCoord(0), true, true) ||
                      util::TestOne("find nearest batch", p1->GetCoord(1) == p2->GetCoord(1), true, true) ||
                      util::TestOne("find nearest batch", dist1 == dist2, true, true)))
                {
                    ok = false;
                    std::cerr << "batch test case failed" << std::endl;
                    break;
                }
            }
        }

        std::cout << (ok ? "Test PASSED" : "Test FAILED! Work harder, you monkey! 🙉") << std::endl;
    }
}