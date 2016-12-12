#ifndef CPPMATH_LINE2_HPP
#define CPPMATH_LINE2_HPP

#include "Vector.hpp"
#include "Point2.hpp"
#include "../TypeTraits.hpp"
#include "../compat.hpp"

/*
 * A 2D Line implementation, based on vectors.
 * p is the starting point, and d is the direction vector.
 * If d is a zero vector then every function has undefined behaviour.
 */

namespace geometry
{
    template <class T>
    class Intersection;

    template <class T>
    class AABB;

    template <class T, bool isray = false>
    class Line2
    {
        public:
            Line2() {};
            Line2(const Point2<T>& p_, const Vec2<T>& d_);
            Line2(const Point2<T>& p1, const Point2<T>& p2);

        public:
            // TODO: Fix this. Until then, leave it disabled.
            // template <bool isray_other>
            // auto isIdentical(const Line2<T, isray_other>& line) const -> bool;

            template <bool isray_other>
            auto isParallel(const Line2<T, isray_other>& line) const -> bool;

            template <bool isray_other>
            auto intersect(const Line2<T, isray_other>& line) const -> Intersection<T>;

            auto intersect(const Point2<T>& p2) const -> bool;
            auto intersect(const AABB<T>& box) const -> Intersection<T>;

        private:
            auto _intersectPointPart(const Vec2<T>& v) const -> bool;

        public:
            Point2<T> p; // start point
            Vec2<T> d; // direction (not normalized)
    };

    typedef Line2<float> Line2f;
    typedef Line2<double> Line2d;
    typedef Line2<int> Line2i;
    typedef Line2<float, true> Ray2f;
    typedef Line2<double, true> Ray2d;
    typedef Line2<int, true> Ray2i;
}

#include "Line2.inl"

#endif
