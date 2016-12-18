#ifndef CPPMATH_LINE2_INL
#define CPPMATH_LINE2_INL

#include "Line2.hpp"
#include "Intersection.hpp"
#include "AABB.hpp"
#include <iostream>
#include <cassert>

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
    Intersection<T> Line2<T>::intersect(const Line2<T>& line) const
    {
        if (d.crossAlmostZero(line.d)) // parallel
            return Intersection<T>();

        const T dx = line.p.x - p.x,
                dy = line.p.y - p.y;
        const double m = d.y * line.d.x - d.x * line.d.y,
                     u = (line.d.x * dy - line.d.y * dx) / m;

        if (_checkScale(u) && line._checkScale((d.x * dy - d.y * dx) / m))
            return Intersection<T>(p + d * u, u);

        return Intersection<T>();
    }

    template <class T>
    Intersection<T> Line2<T>::intersect(const AABB<T>& box) const
    {
        // Adapted from http://noonat.github.io/intersect/#aabb-vs-segment

        // If d contains a zero element, this stores the index of the
        // non-zero element. Otherwise -1.
        int j = -1;

        // Check if an element in d is 0.
        for (size_t i = 0; i < 2; ++i)
        {
            if (almostEquals(d[i], (T)0))
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
            int s = sign(d[j]);
            near.x = (box.pos[j] + ((s == 1) ? 0 : box.size[j]) - p[j]) / d[j];
            near.y = (box.pos[j] + ((s != 1) ? 0 : box.size[j]) - p[j]) / d[j];
        }

        if (type != Line && near.y < 0)
            return Intersection<T>();

        if (type == Segment && near.x > 1)
            return Intersection<T>();

        if (type != Line && near.x < 0)
            near.x = 0;

        if (type == Segment && near.y > 1)
            near.y = 1;

        return Intersection<T>(Point2<T>(p + d * near.x),
                               Point2<T>(p + d * near.y),
                               near);
    }

    template <class T>
    bool Line2<T>::intersect(const Point2<T>& p2) const
    {
        for (size_t i = 0; i < d.size(); ++i)
        {
            if (almostEquals(d[i], (T)0))
            {
                if (!almostEquals(p2[i], p[i]))
                    return false;

                size_t j = (i + 1) % 2;
                return _checkScale((p2[j] - p[j]) / (double)d[j]);
            }
        }

        auto v = (p2 - p) / (Vec2d)d;
        if (!almostEquals(v.x, v.y))
            return false;
        return _checkScale(v.x);
    }

    template <class T>
    inline bool Line2<T>::_checkScale(double u) const
    {
        if (type == Ray)
            return u >= 0;
        if (type == Segment)
            return u >= 0 && u <= 1;
        return true;
    }
}

#endif
