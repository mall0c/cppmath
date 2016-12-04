#ifndef CPPMATH_LINE2_INL
#define CPPMATH_LINE2_INL

#include "Line2.hpp"

namespace geometry
{
    template <class T, bool isray>
    Line2<T, isray>::Line2(const Point2<T>& p_, const Vec2<T>& d_) :
        p(p_), d(d_)
    {};

    template <class T, bool isray>
    Line2<T, isray>::Line2(const Point2<T>& p1, const Point2<T>& p2) :
        p(p1), d(p2 - p1)
    {};

    // TODO: Fix this.
    // template <class T, bool isray>
    // template <bool isray_other>
    // bool Line2<T, isray>::isIdentical(const Line2<T, isray_other>& line) const
    // {
    //     return d.crossAlmostZero(line.d) && (intersect(line.p) || line.intersect(p));
    // }

    template <class T, bool isray>
    template <bool isray_other>
    bool Line2<T, isray>::isParallel(const Line2<T, isray_other>& line) const
    {
        return d.crossAlmostZero(line.d) && !intersect(line.p) && (!isray || !isray_other || !line.intersect(p));
    }

    template <class T, bool isray>
    template <bool isray_other>
    Intersection<T> Line2<T, isray>::intersect(const Line2<T, isray_other>& line) const
    {
        if (d.crossAlmostZero(line.d)) // parallel or identical
            return Intersection<T>();

        const T dx = line.p.x - p.x,
                dy = line.p.y - p.y;
        const double m = d.y * line.d.x - d.x * line.d.y,
                     u = (line.d.x * dy - line.d.y * dx) / m;

        if ((!isray || u >= 0) && (!isray_other || (d.x * dy - d.y * dx) / m >= 0))
            return Intersection<T>(p + d * u);

        return Intersection<T>();
    }

    template <class T, bool isray>
    bool Line2<T, isray>::intersect(const Point2<T>& p2) const
    {
        // If the area of the parallelogram created by these 2 vectors
        // is 0, then they are identical. This means p2 is on this line.
        return _intersectPointPart(p2 - p);
    }

    template <class T, bool isray>
    inline bool Line2<T, isray>::_intersectPointPart(const Vec2<T>& v) const
    {
        return d.crossAlmostZero(v) && (!isray || d.signs() == v.signs());
    }
}

#endif
