#ifndef CPPMATH_AABB_HPP
#define CPPMATH_AABB_HPP

#include "Vector.hpp"

namespace math
{
    template <class T>
    class AABB
    {
        public:
            AABB();
            AABB(T x, T y, T w, T h);
            AABB(const Point2<T>& pos_, const Vec2<T>& size_);
            AABB(const AABB<T>& a, const AABB<T>& b);
            AABB(const AABB<T>& rect) = default;

            static AABB<T> fromPoints(const Point2<T>& a, const Point2<T>& b);

        public:
            void combine(const AABB<T>& other);
            void extend(const AABB<T>& other, bool center = true);
            void extend(const Vec2<T>& size, bool center = true);

            bool isEmpty() const;

            void center(T x, T y);
            void center(const Point2<T>& p);
            Point2<T> getCenter() const;

            bool operator!=(const AABB<T>& r) const;
            bool operator==(const AABB<T>& r) const;

            template <class T2>
            operator AABB<T2>() const;

        public:
            union {
                struct {
                    Vec2<T> pos;
                    Vec2<T> size;
                };
                struct {
                    T x, y, w, h;
                };
            };
    };

    typedef AABB<float> AABBf;
    typedef AABB<int> AABBi;
}

#include "AABB.inl"

#endif
