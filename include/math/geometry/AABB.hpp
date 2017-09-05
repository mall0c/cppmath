#ifndef CPPMATH_AABB_HPP
#define CPPMATH_AABB_HPP

#include "Vector.hpp"
#include "Intersection.hpp"

namespace math
{
    template <class T>
    class AABB
    {
        public:
            AABB();
            AABB(T x, T y, T w, T h);
            AABB(const Vec2<T>& pos_, const Vec2<T>& size_);
            AABB(const AABB<T>& a, const AABB<T>& b);
            AABB(const AABB<T>& rect) = default;

        public:
            void combine(const AABB<T>& other);
            void extend(const AABB<T>& other, bool center = true);

            bool isEmpty() const;

            void center(T x, T y);
            void center(const Point2<T>& p);

            bool contains(const Point2<T>& point) const;
            bool contains(const AABB<T>& rect) const;
            Intersection<T> intersect(const AABB<T>& rect) const;

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
