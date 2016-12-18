#ifndef CPPMATH_TRIANGLE_STRIP_INL
#define CPPMATH_TRIANGLE_STRIP_INL

#include "TriangleStrip.hpp"

namespace geometry
{
    template <typename T>
    TriangleStrip<T>::TriangleStrip(size_t size) :
        Polygon<T>(size)
    { }

    template <typename T>
    template <typename F>
    void TriangleStrip<T>::foreachSegment(F callback) const
    {
        if (_points.size() < 3)
            return;

        callback(_getSegment(0, 1));
        callback(_getSegment(-2, -1));

        for (size_t i = 3; i < _points.size(); i += 2)
            callback(_getSegment(i - 2, i));

        for (size_t i = 2; i < _points.size(); i += 2)
            callback(_getSegment(i - 2, i));
    }


    template <typename T>
    bool TriangleStrip<T>::intersect(const Point2<T>& point) const
    {
        if (_points.size() < 3)
            return false;

        if (!_bbox.contains(point))
            return false;

        Line2<T> ray(point, Vec2<T>(1, 0), Ray);
        size_t num = 0;

        foreachSegment([&](const Line2<T>& line)
        {
            if (ray.intersect(line))
                ++num;
        });

        return num % 2 == 1;
    }

    template <typename T>
    Intersection<T> TriangleStrip<T>::intersect(const Line2<T>& line) const
    {
        if (_points.size() < 3)
            return Intersection<T>();

        if (!line.intersect(_bbox))
            return Intersection<T>();

        Intersection<T> nearest;

        foreachSegment([&](const Line2<T>& seg)
        {
            auto isec = line.intersect(seg);
            if (!nearest || (isec && isec.time < nearest.time))
                nearest = isec;
        });

        return nearest;
    }
}

#endif
