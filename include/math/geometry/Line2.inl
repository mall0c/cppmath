#ifndef CPPMATH_LINE2_INL
#define CPPMATH_LINE2_INL

#include "Line2.hpp"
#include "AABB.hpp"

namespace math
{
    template <class T>
    Line2<T>::Line2(LineType type_) :
        type(type_)
    {};

    template <class T>
    Line2<T>::Line2(const Point2<T>& p_, const Vec2<T>& d_, LineType type_) :
        p(p_), d(d_), type(type_)
    {};

    template <class T>
    Line2<T>::Line2(const Point2<T>& p1, const Point2<T>& p2, LineType type_) :
        p(p1), d(p2 - p1), type(type_)
    {};

    template <class T>
    bool Line2<T>::isParallel(const Line2<T>& line) const
    {
        return d.crossAlmostZero(line.d);
    }

    template <class T>
    Point2<T> Line2<T>::closestPoint(const Point2<T>& point, bool clamp) const
    {
        auto lp = point - p;
        double t = d.dot(lp) / d.abs_sqr();
        if (clamp && type != Line)
        {
            if (type == math::Segment)
                t = std::min(1.0, std::max(0.0, t));
            else if (type == math::Ray)
                t = std::max(0.0, t);
        }
        return p + d * t;
    }

    template <class T>
    T Line2<T>::distance(const Point2<T>& point, bool clamp) const
    {
        return (point - closestPoint(point, clamp)).abs();
    }

    template <class T>
    AABB<T> Line2<T>::getBBox() const
    {
        AABB<T> box(p, d);
        if (d.x < 0)
        {
            box.x += d.x;
            box.w = -box.w;
        }
        if (d.y < 0)
        {
            box.y += d.y;
            box.h = -box.h;
        }
        return box;
    }
}

#endif
