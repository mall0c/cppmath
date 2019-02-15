#ifndef CPPMATH_AABB_INL
#define CPPMATH_AABB_INL

#include "AABB.hpp"

namespace math
{
    template <typename T>
    AABB<T>::AABB() :
        pos(),
        size()
    {};

    template <typename T>
    AABB<T>::AABB(T x, T y, T w, T h) :
        AABB(Point2<T>(x, y), Vec2<T>(w, h))
    {};

    template <typename T>
    AABB<T>::AABB(const Point2<T>& pos_, const Vec2<T>& size_) :
        pos(pos_.asVector()),
        size(size_)
    {};

    template <typename T>
    AABB<T>::AABB(const AABB<T>& a, const AABB<T>& b) :
        AABB(a)
    {
        combine(b);
    }

    template <typename T>
    AABB<T> AABB<T>::fromPoints(const Point2<T>& a, const Point2<T>& b)
    {
        return AABB<T>(
                std::min(a.x, b.x),
                std::min(a.y, b.y),
                std::abs(a.x - b.x),
                std::abs(a.y - b.y)
            );
    }

    template <typename T>
    void AABB<T>::combine(const AABB<T>& other)
    {
        if (isEmpty())
        {
            *this = other;
            return;
        }
        else if (other.isEmpty())
            return;

        Vec2<T> newpos = mins(pos, other.pos);
        size = maxs(pos + size, other.pos + other.size) - newpos;
        pos = newpos;
    }

    template <typename T>
    void AABB<T>::extend(const AABB<T>& other, bool center)
    {
        extend(other.size, center);
    }

    template <typename T>
    void AABB<T>::extend(const Vec2<T>& othersize, bool center)
    {
        if (center)
            pos -= othersize / 2.f;
        size += othersize;
    }

    template <typename T>
    bool AABB<T>::isEmpty() const
    {
        return size.x == 0 && size.y == 0;
    }

    template <typename T>
    void AABB<T>::center(T x, T y)
    {
        pos.fill(x, y);
        pos -= size / 2;
    }

    template <typename T>
    void AABB<T>::center(const Point2<T>& p)
    {
        center(p.x, p.y);
    }

    template <typename T>
    Point2<T> AABB<T>::getCenter() const
    {
        return pos.asPoint() + size / 2;
    }

    template <typename T>
    bool AABB<T>::operator!=(const AABB<T>& r) const
    {
        return pos != r.pos || size != r.size;
    }

    template <typename T>
    bool AABB<T>::operator==(const AABB<T>& r) const
    {
        return pos == r.pos && size == r.size;
    }


    template <typename T>
    template <typename T2>
    AABB<T>::operator AABB<T2>() const
    {
        return AABB<T2>(pos, size);
    }
}

#endif
