#ifndef CPPMATH_AABB_INL
#define CPPMATH_AABB_INL

#include "AABB.hpp"

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
    AABB<T>::AABB(const Point2<T>& pos_, const Vec2<T>& size_) :
        pos(pos_.asVector()),
        size(size_)
    {};

    template <class T>
    AABB<T>::AABB(const AABB<T>& a, const AABB<T>& b) :
        AABB(a)
    {
        combine(b);
    }

    template <class T>
    AABB<T>::AABB(const Point2<T>& a, const Point2<T>& b) :
        AABB(
                std::min(a.x, b.x),
                std::min(a.y, b.y),
                std::abs(a.x - b.x),
                std::abs(a.y - b.y)
            )
    { }

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
        extend(other.size, center);
    }

    template <class T>
    void AABB<T>::extend(const Vec2<T>& othersize, bool center)
    {
        if (center)
            pos -= othersize / 2.f;
        size += othersize;
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
    Point2<T> AABB<T>::getCenter() const
    {
        return pos.asPoint() + size / 2;
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
