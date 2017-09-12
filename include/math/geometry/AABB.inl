#ifndef CPPMATH_AABB_INL
#define CPPMATH_AABB_INL

#include <cassert>
#include "AABB.hpp"
#include "../math.hpp"

namespace math
{
    template <class T>
    AABB<T>::AABB() :
        pos(),
        size()
    {};

    template <class T>
    AABB<T>::AABB(T x, T y, T w, T h) :
        pos(x, y),
        size(w, h)
    {};

    template <class T>
    AABB<T>::AABB(const Vec2<T>& pos_, const Vec2<T>& size_) :
        pos(pos_),
        size(size_)
    {};

    template <class T>
    AABB<T>::AABB(const AABB<T>& a, const AABB<T>& b) :
        AABB(a)
    {
        combine(b);
    }

    template <class T>
    void AABB<T>::combine(const AABB<T>& other)
    {
        if (isEmpty())
        {
            *this = other;
            return;
        }
        else if (other.isEmpty())
            return;

        T newx = std::min(x, other.x),
          newy = std::min(y, other.y);
        w = std::max(x + w, other.x + other.w) - newx;
        h = std::max(y + h, other.y + other.h) - newy;
        x = newx;
        y = newy;
    }

    template <class T>
    void AABB<T>::extend(const AABB<T>& other, bool center)
    {
        if (center)
            pos -= other.size / 2;
        size += other.size;
    }

    template <class T>
    bool AABB<T>::isEmpty() const
    {
        return size.x == 0 && size.y == 0;
    }

    template <class T>
    void AABB<T>::center(T x, T y)
    {
        pos.fill(x, y);
        pos -= size / 2;
    }

    template <class T>
    void AABB<T>::center(const Point2<T>& p)
    {
        center(p.x, p.y);
    }

    template <class T>
    Vec2<T> AABB<T>::getCenter() const
    {
        return pos + size / 2;
    }

    template <class T>
    bool AABB<T>::contains(const Point2<T>& point) const
    {
        return point.asVector() >= pos && point.asVector() < pos + size;
    }

    template <class T>
    bool AABB<T>::contains(const AABB<T>& rect) const
    {
        return contains(rect.pos) && contains(rect.pos + rect.size);
    }

    template <class T>
    Intersection<T> AABB<T>::intersect(const AABB<T>& other) const
    {
        // Adapted from http://noonat.github.io/intersect/#aabb-vs-aabb
        T dx = (other.pos.x + other.size.x / 2) - (pos.x + size.x / 2);
        T px = (other.size.x / 2 + size.x / 2) - std::abs(dx);
        if (px <= 0)
            return Intersection<T>();

        T dy = (other.pos.y + other.size.y / 2) - (pos.y + size.y / 2);
        T py = (other.size.y / 2 + size.y / 2) - std::abs(dy);
        if (py <= 0)
            return Intersection<T>();

        return Intersection<T>(
                Vec2<T>(px * (dx < 0 ? -1 : 1), py * (dy < 0 ? -1 : 1)),
                px < py ? Vec2<T>(-sign(dx), 0) : Vec2<T>(0, -sign(dy)));
    };

    template <class T>
    Intersection<T> AABB<T>::sweep(const Vec2<T>& vel, AABB<T> box) const
    {
        box.extend(*this);
        return Line2<T>(getCenter().asPoint(), vel, Segment).intersect(box);
    }

    template <class T>
    Intersection<T> AABB<T>::sweep(const Vec2<T>& vel, const Polygon<T>& pol) const
    {
        Intersection<T> nearest;
        pol.foreachSegment([&](const Line2<T>& seg) {
                auto isec = sweep(vel, seg);
                if (!nearest || (isec && isec.time < nearest.time))
                    nearest = isec;
                return false;
            });
        return nearest;
    }

    template <class T>
    Intersection<T> AABB<T>::sweep(const Vec2<T>& vel, const Line2<T>& line) const
    {
        // Based on https://gamedev.stackexchange.com/questions/29479/swept-aabb-vs-line-segment-2d
        // Praise OP

        // TODO: support rays
        assert(line.type != Ray && "Rays are not supported yet");

        auto half = size / 2;
        auto center = pos + half;
        auto ln = line.d.ortho().normalized(); // line normal
        auto dist = ln.dot(line.p.asVector() - center);

        if (dist < 0)
            ln *= -1;

        dist = std::abs(dist);
        auto r = half.x * std::abs(ln.x) + half.y * std::abs(ln.y);
        auto velproj = ln.dot(vel);

        if (velproj < 0)
            r *= -1;

        Vec2<T> times(std::max((dist - r) / velproj, 0.0f),
                      std::min((dist + r) / velproj, 1.0f));

        if (line.type == Segment)
        { // AABB vs AABB sweep
            AABB<T> linebox = line.getBBox();
            Vec2<T> aabbmax = pos + size;
            Vec2<T> lineMax = linebox.pos + linebox.size;

            // X axis overlap
            if (vel.x < 0) //Sweeping left
            {
                if (aabbmax.x < linebox.pos.x)
                    return Intersection<T>();
                times[0] = std::max((lineMax.x - pos.x) / vel.x, times[0]);
                times[1] = std::min((linebox.pos.x - aabbmax.x) / vel.x, times[1]);
            }
            else if (vel.x > 0) //Sweeping right
            {
                if (pos.x > lineMax.x)
                    return Intersection<T>();
                times[0] = std::max((linebox.pos.x - aabbmax.x) / vel.x, times[0]);
                times[1] = std::min((lineMax.x - pos.x) / vel.x, times[1]);
            }
            else
            {
                if (linebox.pos.x > aabbmax.x || lineMax.x < pos.x)
                    return Intersection<T>();
            }

            if (times[0] > times[1])
                return Intersection<T>();

            // Y axis overlap
            if (vel.y < 0) //Sweeping down
            {
                if (aabbmax.y < linebox.pos.y)
                    return Intersection<T>();
                times[0] = std::max((lineMax.y - pos.y) / vel.y, times[0]);
                times[1] = std::min((linebox.pos.y - aabbmax.y) / vel.y, times[1]);
            }
            else if (vel.y > 0) //Sweeping up
            {
                if (pos.y > lineMax.y)
                    return Intersection<T>();
                times[0] = std::max((linebox.pos.y - aabbmax.y) / vel.y, times[0]);
                times[1] = std::min((lineMax.y - pos.y) / vel.y, times[1]);
            }
            else
            {
                if (linebox.pos.y > aabbmax.y || lineMax.y < pos.y)
                    return Intersection<T>();
            }
        }

        if (times[0] > times[1])
            return Intersection<T>();

        return Intersection<T>((center + vel * times[0]).asPoint(), times, -ln);
    }

    template <class T>
    bool AABB<T>::operator!=(const AABB<T>& r) const
    {
        return pos != r.pos || size != r.size;
    }

    template <class T>
    bool AABB<T>::operator==(const AABB<T>& r) const
    {
        return pos == r.pos && size == r.size;
    }


    template <class T>
    template <class T2>
    AABB<T>::operator AABB<T2>() const
    {
        return AABB<T2>(pos, size);
    }
}

#endif
