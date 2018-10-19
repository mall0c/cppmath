#ifndef CPPMATH_POLYGON_INL
#define CPPMATH_POLYGON_INL

#include <cassert>
#include "Polygon.hpp"
#include "LineStrip.hpp"
#include "TriangleStrip.hpp"

namespace math
{
    template <typename T>
    BasePolygon<T>::BasePolygon(PolygonType type_, NormalDirection ndir) :
        type(type_),
        normaldir(ndir)
    { }

    template <typename T>
    BasePolygon<T>::BasePolygon(size_t size, PolygonType type_, NormalDirection ndir) :
        BasePolygon(type_, ndir)
    {
        _points.reserve(size);
    }

    template <typename T>
    void BasePolygon<T>::add(const Point2<T>& point)
    {
        addRaw(point);
    }

    template <typename T>
    void BasePolygon<T>::addRaw(const Point2<T>& point)
    {
        _points.push_back(point);

        if (!_bbox.contains(point))
            _recalculate();
    }

    template <typename T>
    void BasePolygon<T>::edit(size_t i, const Point2<T>& p)
    {
        editRaw(i, p);
    }

    template <typename T>
    void BasePolygon<T>::editRaw(size_t i, const Point2<T>& p)
    {
        auto& pold = _getRaw(i);
        if (p != pold)
        {
            pold = p;
            _recalculate();
        }
    }

    template <typename T>
    void BasePolygon<T>::clear()
    {
        _points.clear();
        _recalculate();
    }

    template <typename T>
    Point2<T> BasePolygon<T>::get(int i) const
    {
        return getRaw(i);
    }

    template <typename T>
    const Point2<T>& BasePolygon<T>::getRaw(int i) const
    {
        return _points[(i < 0) ? _points.size() + i : i];
    }

    template <typename T>
    Line2<T> BasePolygon<T>::getSegment(int i, int j, bool raw) const
    {
        return Line2<T>(raw ? getRaw(i) : get(i), raw ? getRaw(j) : get(j), Segment);
    }

    template <typename T>
    template <typename F>
    void BasePolygon<T>::foreachSegment(F callback, bool raw) const
    {
        switch (type)
        {
            case LineStrip:
                foreachSegmentLineStrip(*this, callback, raw);
                break;
            case TriangleStrip:
                foreachSegmentTriangleStrip(*this, callback, raw);
                break;
            default:
                assert(false && "currently not supported");
        }
    }

    template <typename T>
    Intersection<T> BasePolygon<T>::intersect(const Line2<T>& line, bool convex, bool invert) const
    {
        switch (type)
        {
            case LineStrip:
                return intersectLineStrip(*this, line);
            case TriangleStrip:
                return intersectTriangleStrip(*this, line, convex, invert);
            default:
                assert(false && "currently not supported");
        }
        return Intersection<T>();
    }

    template <typename T>
    bool BasePolygon<T>::intersect(const Point2<T>& point, bool convex, bool invert) const
    {
        switch (type)
        {
            case LineStrip:
                return intersectLineStrip(*this, point);
            case TriangleStrip:
                return intersectTriangleStrip(*this, point, convex, invert);
            default:
                assert(false && "currently not supported");
        }
        return false;
    }

    template <typename T>
    Intersection<T> BasePolygon<T>::findNearest(const Line2<T>& line) const
    {
        Intersection<T> nearest;

        auto cb = [&](const Line2<T>& seg) {
            auto isec = line.intersect(seg, normaldir);
            if (!nearest || (isec && isec.time < nearest.time))
                nearest = isec;
            return false;
        };

        foreachSegment(cb);
        return nearest;
    }


    template <typename T>
    Point2<T>& BasePolygon<T>::_getRaw(int i)
    {
        return _points[(i < 0) ? _points.size() + i : i];
    }

    template <typename T>
    const AABB<T>& BasePolygon<T>::getBBox() const
    {
        return _bbox;
    }

    template <typename T>
    size_t BasePolygon<T>::size() const
    {
        return _points.size();
    }


    template <typename T>
    void BasePolygon<T>::_recalculate()
    {
        if (_points.size() < 2)
        {
            _bbox = AABB<T>();
            return;
        }

        Vec2f min = get(0).asVector(),
              max = get(0).asVector();

        for (size_t i = 1; i < _points.size(); ++i)
        {
            auto p = get(i);
            for (int k = 0; k < 2; ++k)
            {
                min[k] = std::min(min[k], p[k]);
                max[k] = std::max(max[k], p[k]);
            }
        }

        _bbox.pos = min;
        _bbox.size = max - min;
    }



    template <typename T>
    Polygon<T>::Polygon(PolygonType type, NormalDirection ndir) :
        BasePolygon<T>(type, ndir),
        _scale(1, 1)
    { }

    template <typename T>
    Polygon<T>::Polygon(size_t size, PolygonType type, NormalDirection ndir) :
        BasePolygon<T>(size, type, ndir),
        _scale(1, 1)
    { }

    template <typename T>
    void Polygon<T>::add(const Point2<T>& point)
    {
        this->addRaw(((point - _offset).asVector() / _scale).asPoint());
    }

    template <typename T>
    void Polygon<T>::edit(size_t i, const Point2<T>& p)
    {
        this->editRaw(i, ((p - _offset).asVector() / _scale).asPoint());
    }

    template <typename T>
    Point2<T> Polygon<T>::get(int i) const
    {
        return (this->getRaw(i).asVector() * _scale + _offset).asPoint();
    }

    template <typename T>
    void Polygon<T>::setOffset(const Vec2<T>& off)
    {
        move(off - _offset);
    }

    template <typename T>
    void Polygon<T>::move(const Vec2<T>& rel)
    {
        _offset += rel;
        this->_bbox.pos += rel;
    }

    template <typename T>
    const Vec2<T>& Polygon<T>::getOffset() const
    {
        return _offset;
    }

    template <typename T>
    void Polygon<T>::setScale(const Vec2<T>& scale)
    {
        _scale = scale;
        this->_recalculate();
    }

    template <typename T>
    const Vec2<T>& Polygon<T>::getScale() const
    {
        return _scale;
    }
}

#endif
