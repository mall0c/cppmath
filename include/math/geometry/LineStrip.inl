#ifndef CPPMATH_LINE_STRIP_INL
#define CPPMATH_LINE_STRIP_INL

#include "LineStrip.hpp"

namespace math
{
    template <typename T>
    LineStrip<T>::LineStrip(size_t size) :
        Polygon<T>(size)
    { }

    template <typename T>
    template <typename F>
    void LineStrip<T>::foreachSegment(F callback) const
    {
        for (size_t i = 1; i < _points.size(); ++i)
            callback(_getSegment(i - 1, i));
    }


    template <typename T>
    bool LineStrip<T>::intersect(const Point2<T>& point) const
    {
        if (_points.size() < 2)
            return false;

        if (!_bbox.contains(point))
            return false;

        // Don't use foreachSegment, to be able to return early
        for (size_t i = 1; i < _points.size(); i += 1)
            if (_getSegment(i - 1, i).intersect(point))
                return true;

        return false;
    }

    template <typename T>
    Intersection<T> LineStrip<T>::intersect(const Line2<T>& line) const
    {
        if (_points.size() < 2)
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
