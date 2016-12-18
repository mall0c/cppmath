#ifndef CPPMATH_LINE2_HPP
#define CPPMATH_LINE2_HPP

#include "Vector.hpp"
#include "Point2.hpp"
#include "../TypeTraits.hpp"
#include "../compat.hpp"

/*
 * A 2D Line implementation, based on vectors.
 * p is the starting point, and d is the direction vector.
 * If d is a zero vector, every function has undefined behaviour.
 * A line can have different types:
 *  Line:       Goes to infinity on both ends
 *  Ray:        Goes to infinity in the direction of d
 *  Segment:    Has a fixed size d
 */

namespace math
{
    template <class T>
    class Intersection;

    template <class T>
    class AABB;

    enum LineType : char
    {
        Line,
        Ray,
        Segment
    };

    template <class T>
    class Line2
    {
        public:
            Line2(LineType type_ = Line);
            Line2(const Point2<T>& p_, const Vec2<T>& d_, LineType type_ = Line);
            Line2(const Point2<T>& p1, const Point2<T>& p2, LineType type_ = Segment);

        public:
            auto isParallel(const Line2<T>& line) const -> bool;

            auto intersect(const Line2<T>& line) const -> Intersection<T>;
            auto intersect(const Point2<T>& p2) const -> bool;
            auto intersect(const AABB<T>& box) const -> Intersection<T>;

        private:
            auto _checkScale(double u) const -> bool;

        public:
            Point2<T> p; // start point
            Vec2<T> d; // direction (not normalized)
            LineType type;
    };

    typedef Line2<float> Line2f;
    typedef Line2<double> Line2d;
    typedef Line2<int> Line2i;
}

#include "Line2.inl"

#endif
