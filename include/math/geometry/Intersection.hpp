#ifndef CPPMATH_INTERSECTION_HPP
#define CPPMATH_INTERSECTION_HPP

#include "Vector.hpp"
#include "Point2.hpp"

namespace geometry
{
    template <class T>
    class Intersection
    {
        public:
            enum Type
            {
                None,
                LinexLine,
                LinexAABB,
                AABBxAABB
            };

        public:
            Intersection() : type(None) {}
            Intersection(Type type_) : type(type_) {}
            Intersection(const Vec2<T>& d) : type(AABBxAABB), delta(d) {}
            Intersection(const Point2<T>& p_) : type(LinexLine), p(p_) {}

            operator bool() const
            {
                return type != None;
            }

        public:
            Type type;
            union {
                Vec2<T> delta;
                Point2<T> p;
            };
    };
}

#endif
