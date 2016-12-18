#ifndef CPPMATH_POLYGON_INL
#define CPPMATH_POLYGON_INL

#include "Polygon.hpp"

namespace geometry
{
    template <typename T>
    Polygon<T>::Polygon(size_t size)
    {
        _points.reserve(size);
    }

    template <typename T>
    void Polygon<T>::add(const Point2<T>& point)
    {
        _points.push_back(point - _offset);

        if (_points.size() >= 3 && !_bbox.contains(get(-1)))
            _recalculate();
    }

    template <typename T>
    void Polygon<T>::edit(size_t i, const Point2<T>& p)
    {
        _points[i] = p - _offset;
        _recalculate();
    }

    template <typename T>
    Point2<T> Polygon<T>::get(int i) const
    {
        return getRaw(i) + _offset;
    }

    template <typename T>
    const Point2<T>& Polygon<T>::getRaw(int i) const
    {
        return _points[(i < 0) ? _points.size() + i : i];
    }


    template <typename T>
    void Polygon<T>::setOffset(T x, T y)
    {
        _bbox.pos -= _offset;
        _offset.fill(x, y);
        _bbox.pos += _offset;
    }

    template <typename T>
    void Polygon<T>::move(T x, T y)
    {
        _offset.x += x;
        _offset.y += y;
        _bbox.pos.x += x;
        _bbox.pos.y += y;
    }


    template <typename T>
    const AABB<T>& Polygon<T>::getBBox() const
    {
        return _bbox;
    }

    template <typename T>
    size_t Polygon<T>::size() const
    {
        return _points.size();
    }


    template <typename T>
    void Polygon<T>::_recalculate()
    {
        if (_points.size() < 3)
        {
            _bbox = AABB<T>();
            return;
        }

        Vec2f min = _points[0].asVector(),
              max = _points[0].asVector();

        for (size_t i = 1; i < _points.size(); ++i)
        {
            for (int k = 0; k < 2; ++k)
            {
                min[k] = std::min(min[k], _points[i][k]);
                max[k] = std::max(max[k], _points[i][k]);
            }
        }

        _bbox.pos = _offset + min;
        _bbox.size = max - min;
    }

    template <typename T>
    Line2<T> Polygon<T>::_getSegment(int i, int j) const
    {
        return Line2<T>(get(i), get(j), Segment);
    }
}

#endif
