#pragma once

#include <memory>
#include <vector>
#include <string>

namespace QuadTree {

    using namespace std;

    struct TPoint {
        float X;
        float Y;

        TPoint()
            : X(0)
            , Y(0)
        {}

        TPoint(float x, float y)
            : X(x)
            , Y(y)
        {}

        bool operator==(const TPoint & rhs) const {
            return X == rhs.X && Y == rhs.Y;
        }
    };

    struct TCircle {
        TPoint Center;
        float Radius;

        TCircle(const TPoint & center, float radius)
            : Center(center)
            , Radius(radius)
        {}

        bool Contains(const TPoint & p) const {
            return sqrt((p.X - Center.X)*(p.X - Center.X) + (p.Y - Center.Y)*(p.Y - Center.Y)) <= Radius;
        }
    };

    struct TBoundingBox {
        TPoint Center;
        float HalfDimension;

        TBoundingBox(const TPoint & center, float halfDimension)
            : Center(center)
            , HalfDimension(halfDimension)
        {}

        bool Contains(const TPoint & p) const {
            return abs(Center.X - p.X) <= HalfDimension && abs(Center.Y - p.Y) <= HalfDimension;
        }

        bool HasIntersection(const TBoundingBox & bbox) const {
            return Contains(TPoint(bbox.Center.X - bbox.HalfDimension, bbox.Center.Y - bbox.HalfDimension)) ||
                   Contains(TPoint(bbox.Center.X - bbox.HalfDimension, bbox.Center.Y + bbox.HalfDimension)) ||
                   Contains(TPoint(bbox.Center.X + bbox.HalfDimension, bbox.Center.Y - bbox.HalfDimension)) ||
                   Contains(TPoint(bbox.Center.X + bbox.HalfDimension, bbox.Center.Y + bbox.HalfDimension));
        }

        // Checks intersection with circle's bounding box (for simplicity).
        // This is fine in terms of quad tree correctness yet performance is not optimal.
        bool HasIntersection(const TCircle c) const {
            TBoundingBox bbox(c.Center, c.Radius);
            return HasIntersection(bbox) || bbox.HasIntersection(*this);
        }

        vector<TBoundingBox> Split() const {
            return { NW(), NE(), SW(), SE() };
        }

    private:
        TBoundingBox NW() const {
            return TBoundingBox(TPoint(Center.X - HalfDimension / 2, Center.Y + HalfDimension / 2), HalfDimension / 2);
        }

        TBoundingBox NE() const {
            return TBoundingBox(TPoint(Center.X + HalfDimension / 2, Center.Y + HalfDimension / 2), HalfDimension / 2);
        }

        TBoundingBox SW() const {
            return TBoundingBox(TPoint(Center.X - HalfDimension / 2, Center.Y - HalfDimension / 2), HalfDimension / 2);
        }

        TBoundingBox SE() const {
            return TBoundingBox(TPoint(Center.X + HalfDimension / 2, Center.Y - HalfDimension / 2), HalfDimension / 2);
        }
    };

    struct TPlace {
        TPoint Coord;
        string Name;

        TPlace(const TPoint & coord, const string & name)
            : Coord(coord)
            , Name(name)
        {}

        TPlace(const TPlace & p)
            : Coord(p.Coord)
            , Name(p.Name)
        {}

        bool operator==(const TPlace & rhs) const {
            return Coord == rhs.Coord && Name == rhs.Name;
        }
    };

    class TQuadTree {
    public:
        TQuadTree(const TBoundingBox & bbox, size_t maxNodeCapacity)
            : BBox(bbox)
            , MaxNodeCapacity(maxNodeCapacity)
        {}

        bool Add(const TPlace & p) {
            if (!BBox.Contains(p.Coord))
                return false;

            if (Children.empty()) {
                if (Places.size() < MaxNodeCapacity) {
                    Places.push_back(p);
                    return true;
                }

                Subdivide();
            }

            for (auto & child : Children) {
                if (child->Add(p))
                    return true;
            }

            // Should never happen
            return false;
        }

        void Find(const TCircle & c, vector<TPlace> & res) const {
            if (!BBox.HasIntersection(c))
                return;

            if (!Children.empty()) {
                for (const auto & child : Children) {
                    child->Find(c, res); 
                }
                return;
            }

            for (const auto & p : Places) {
                if (c.Contains(p.Coord))
                    res.push_back(p);
            }

        }

    private:
        void Subdivide() {
            for (const auto & bbox : BBox.Split()) {
                Children.push_back(make_unique<TQuadTree>(bbox, MaxNodeCapacity));
            }

            for (const auto & p : Places) {
                for (auto & child : Children) {
                    if (child->Add(p))
                        break;
                }
            }            
            
            // Clear Points vector
            vector<TPlace>().swap(Places);
        }


    private:
        TBoundingBox BBox;
        size_t MaxNodeCapacity;
        vector<TPlace> Places;

        vector<unique_ptr<TQuadTree>> Children;
    };

    void Run() {
        TQuadTree quadtree(TBoundingBox(TPoint(0, 0), 5), 4);

        TPlace spaceNeedle(TPoint(2, 2), "Space Needle");
        TPlace pizzaHut(TPoint(1, 1), "Pizza Hut");
        TPlace starbucks(TPoint(2, 1), "Starbucks");
        TPlace papaJohns(TPoint(4, 5), "Papa John's");

        TPlace wildGinger(TPoint(-3, -4), "Wild Ginger");
        TPlace trullys(TPoint(-2, -1), "Trully's");

        vector<TPlace> places = { spaceNeedle, pizzaHut, starbucks, papaJohns, wildGinger, trullys };

        for (const auto & p : places) {
            util::TestOne(p.Name, quadtree.Add(p), true);
        }

        vector<TPlace> res;
        quadtree.Find(TCircle(TPoint(0, 0), 100), res);
        util::TestOne("radius 100 @ (0; 0)", res == places, true);
        util::TestOne("radius 100 @ (0; 0)", res.size(), places.size());

        res.clear();
        quadtree.Find(TCircle(TPoint(2, 1), 1), res);
        util::TestOne("radius 1 @ (2; 1)", res == vector<TPlace>({spaceNeedle, pizzaHut, starbucks}), true);
        util::TestOne("radius 1 @ (2; 1)", res.size(), 3ul);

        res.clear();
        quadtree.Find(TCircle(TPoint(3, -3), 3), res);
        util::TestOne("radius 3 @ (3; -3)", res == vector<TPlace>(), true);
        util::TestOne("radius 3 @ (3; -3)", res.size(), 0ul);

        res.clear();
        quadtree.Find(TCircle(TPoint(-3, -2), 2), res);
        util::TestOne("radius 2 @ (-3; -2)", res == vector<TPlace>({wildGinger, trullys}), true);
        util::TestOne("radius 2 @ (-3; -2)", res.size(), 2ul);
    }
}