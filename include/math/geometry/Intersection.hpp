#ifndef CPPMATH_INTERSECTION_HPP
#define CPPMATH_INTERSECTION_HPP

#include "Vector.hpp"
#include "Point2.hpp"
#include "Line2.hpp"

namespace math
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

            // AABB vs AABB
            Intersection(const Vec2<T>& d, const Vec2<T>& normal_) :
                type(AABBxAABB), normal(normal_), delta(d) {}

            // Line vs Line
            Intersection(const Point2<T>& p_, const Vec2<T>& times_, const Vec2<T>& normal_) :
                type(LinexLine), normal(normal_), times(times_), p(p_) {}

            // Line vs AABB
            Intersection(const Point2<T>& p1, const Point2<T>& p2, const Vec2<T>& times_, const Vec2<T>& normal_) :
                type(LinexAABB), normal(normal_), times(times_), seg(p1, p2, Segment) {}

            operator bool() const
            {
                return type != None;
            }

        public:
            IntersectionType type;
            Vec2<T> normal;
            union {
                T time;
                Vec2<T> times;
                struct {
                    T near;
                    T far;
                };
            };
            union {
                // Put these in a struct so they have the same memory layout
                // as Line2, which might come in handy some day.
                struct {
                    Point2<T> p;    // Line vs Line
                    Vec2<T> delta;  // AABB vs AABB
                };
                Line2<T> seg;   // Line vs AABB
            };
    };
}

#endif
