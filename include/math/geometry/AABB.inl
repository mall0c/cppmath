#ifndef CPPMATH_AABB_INL
#define CPPMATH_AABB_INL

#include "AABB.hpp"
#include "../math.hpp"

namespace math
{
    template <class T>
    AABB<T>::AABB() {};

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
    void AABB<T>::crop(const AABB<T>& rect)
    {
        if (intersects(rect))
        {
            size += pos; //convert to "range"-rect

            size.x = std::min(size.x, rect.pos.x + rect.size.x);
            size.y = std::min(size.y, rect.pos.y + rect.size.y);

            pos.x = std::max(pos.x, rect.pos.x);
            pos.y = std::max(pos.y, rect.pos.y);

            size -= pos;
        }
        else
        {
            pos = size = Vec2<T>(); //null rect
        }
    };

    template <class T>
    void AABB<T>::center(T x, T y)
    {
        pos.fill(x, y);
    }

    template <class T>
    void AABB<T>::center(const Point2<T>& p)
    {
        center(p.x, p.y);
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
        T px = (other.size.x / 2 + size.x / 2) - abs(dx);
        if (px <= 0)
            return Intersection<T>();

        T dy = (other.pos.y + other.size.y / 2) - (pos.y + size.y / 2);
        T py = (other.size.y / 2 + size.y / 2) - abs(dy);
        if (py <= 0)
            return Intersection<T>();

        return Intersection<T>(Vec2<T>(
                    px * (dx < 0 ? -1 : 1), py * (dy < 0 ? -1 : 1)));
    };


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
