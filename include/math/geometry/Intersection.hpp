#ifndef CPPMATH_INTERSECTION_HPP
#define CPPMATH_INTERSECTION_HPP

#include "Vector.hpp"
#include "Point2.hpp"
#include "Line2.hpp"

namespace geometry
{
    enum IntersectionType
    {
        None,
        LinexLine,
        LinexAABB,
        AABBxAABB
    };

    template <class T>
    class Intersection
    {
        public:
            Intersection() : type(None) {}
            Intersection(IntersectionType type_) : type(type_) {}
            Intersection(const Vec2<T>& d) : type(AABBxAABB), delta(d) {}
            Intersection(const Point2<T>& p_) : type(LinexLine), p(p_) {}
            Intersection(const Point2<T>& p1, const Point2<T>& p2) :
                type(LinexAABB), seg(p1, p2, Segment) {}

            operator bool() const
            {
                return type != None;
            }

        public:
            IntersectionType type;
            union {
                // Put these in a struct so they have the same memory layout
                // as Line2, which might come in handy some day.
                struct {
                    Point2<T> p;
                    Vec2<T> delta;
                };
                Line2<T> seg;
            };
    };
}

#endif
