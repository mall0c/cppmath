#ifndef CPPMATH_LINE2_HPP
#define CPPMATH_LINE2_HPP

#include "Vector.hpp"
#include "Point2.hpp"
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
    class AABB;

    enum LineType
    {
        Line,
        Ray,
        Segment
    };

    enum NormalDirection
    {
        NormalBoth,
        NormalLeft,
        NormalRight
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

            auto closestPoint(const Point2<T>& point, bool clamp = true) const -> Point2<T>;
            auto distance(const Point2<T>& point, bool clamp = true) const     -> T;

            // Only useful for segments
            auto getBBox() const -> AABB<T>;

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
