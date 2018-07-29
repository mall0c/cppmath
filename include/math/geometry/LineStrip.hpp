#ifndef CPPMATH_LINE_STRIP_HPP
#define CPPMATH_LINE_STRIP_HPP

#include "Polygon.hpp"
#include "Intersection.hpp"

namespace math
{
    // Callback signature: (const Line2<T>&) -> bool
    // Returning true breaks the loop.
    template <typename T, typename F>
    auto foreachSegmentLineStrip(const Polygon<T>& polygon, F callback) -> void
    {
        for (size_t i = 1; i < polygon.size(); ++i)
            if (callback(polygon.getSegment(i - 1, i)))
                return;
    }

    template <typename T>
    auto intersectLineStrip(const Polygon<T>& polygon, const Line2<T>& line) -> Intersection<T>
    {
        if (polygon.size() < 2)
            return Intersection<T>();

        if (!line.intersect(polygon.getBBox()))
            return Intersection<T>();

        return polygon.findNearest(line);
    }

    template <typename T>
    auto intersectLineStrip(const Polygon<T>& polygon, const Point2<T>& point) -> bool
    {
        if (polygon.size() < 2)
            return false;

        if (!polygon.getBBox().contains(point))
            return false;

        for (size_t i = 1; i < polygon.size(); ++i)
            if (polygon.getSegment(i - 1, i).intersect(point))
                return true;

        return false;
    }
}

#endif
