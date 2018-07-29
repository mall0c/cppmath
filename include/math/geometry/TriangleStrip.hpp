#ifndef CPPMATH_TRIANGLE_STRIP_HPP
#define CPPMATH_TRIANGLE_STRIP_HPP

#include "Polygon.hpp"
#include "Intersection.hpp"

namespace math
{
    // Callback signature: (const Line2<T>&) -> bool
    // Returning true breaks the loop.
    template <typename T, typename F>
    auto foreachSegmentTriangleStrip(const Polygon<T>& polygon, F callback) -> void
    {
        if (polygon.size() < 3)
            return;

        if (callback(polygon.getSegment(0, 1)))
            return;
        if (callback(polygon.getSegment(-2, -1)))
            return;

        for (size_t i = 3; i < polygon.size(); i += 2)
            if (callback(polygon.getSegment(i - 2, i)))
                return;

        for (size_t i = 2; i < polygon.size(); i += 2)
            if (callback(polygon.getSegment(i - 2, i)))
                return;
    }

    template <typename T>
    auto intersectTriangleStrip(const Polygon<T>& polygon, const Line2<T>& line, bool convex, bool invert = false) -> Intersection<T>
    {
        if (polygon.size() < 3)
            return Intersection<T>();

        if (!line.intersect(polygon.getBBox()))
        {
            if (invert)
                return Intersection<T>(line.p, Vec2f(), Vec2f());
            else
                return Intersection<T>();
        }

        // intersectTriangleStrip(point) already is invert dependent
        if (line.type != Line && intersectTriangleStrip(polygon, line.p, convex, invert))
            return Intersection<T>(line.p, Vec2f(), Vec2f());

        return polygon.findNearest(line);
    }

    template <typename T>
    auto intersectTriangleStrip(const Polygon<T>& polygon, const Point2<T>& point, bool convex, bool invert = false) -> bool
    {
        if (polygon.size() < 3)
            return false;

        if (!polygon.getBBox().contains(point))
            return invert;

        Line2<T> ray(point, Vec2<T>(1, 0), Ray);
        size_t num = 0;

        foreachSegmentTriangleStrip(polygon, [&](const Line2<T>& line)
        {
            if (ray.intersect(line))
                ++num;
            return convex ? num == 2 : false;
        });

        return num % 2 == (invert ? 0 : 1);
    }
}

#endif
