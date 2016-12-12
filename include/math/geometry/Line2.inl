#ifndef CPPMATH_LINE2_INL
#define CPPMATH_LINE2_INL

#include "Line2.hpp"
#include "Intersection.hpp"
#include "AABB.hpp"
#include <iostream>
#include <cassert>

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
    Intersection<T> Line2<T, isray>::intersect(const AABB<T>& box) const
    {
        // Adapted from http://noonat.github.io/intersect/#aabb-vs-segment

        // If d contains a zero element, this stores the index of the
        // non-zero element. Otherwise -1.
        int j = -1;

        // Check if an element in d is 0.
        for (size_t i = 0; i < 2; ++i)
        {
            if (math::almostEquals(d[i], (T)0))
            {
                if (p[i] >= box.pos[i] && p[i] < box.pos[i] + box.size[i])
                {
                    j = (i + 1) % 2;
                    break;
                }
                return Intersection<T>();
            }
        }

        // near will be reused to store the final near and far time.
        Vec2d near;

        if (j == -1)
        {
            auto signs = d.signs();
            Vec2d half = box.size / 2.0;
            near      = ((box.pos + half) - signs * half - p.asVector()) / d;
            Vec2d far = ((box.pos + half) + signs * half - p.asVector()) / d;

            if (near.x > far.y || near.y > far.x)
                return Intersection<T>();

            near.x = std::max(near.x, near.y); // near time
            near.y = std::min(far.x, far.y); // far time
        }
        else
        {
            int s = math::sign(d[j]);
            near.x = (box.pos[j] + ((s == 1) ? 0 : box.size[j]) - p[j]) / d[j];
            near.y = (box.pos[j] + ((s != 1) ? 0 : box.size[j]) - p[j]) / d[j];
        }

        Point2<T> p1(p + d * near.x),
                  p2(p + d * near.y);

        // Ray check
        if (isray && near.x < 0)
        {
            if (near.y >= 0)
                p1 = p;
            else
                return Intersection<T>();
        }

        // TODO: Line segment check

        return Intersection<T>(p1, p2 - p1);
    }

    template <class T, bool isray>
    inline bool Line2<T, isray>::_intersectPointPart(const Vec2<T>& v) const
    {
        return d.crossAlmostZero(v) && (!isray || d.signs() == v.signs());
    }
}

#endif
