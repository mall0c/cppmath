#ifndef CPPMATH_AABB_HPP
#define CPPMATH_AABB_HPP

#include "Vector.hpp"
#include "Intersection.hpp"

namespace math
{
    template <class T>
    class Polygon;

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
            auto combine(const AABB<T>& other)                    -> void;
            auto extend(const AABB<T>& other, bool center = true) -> void;

            auto isEmpty() const -> bool;

            auto center(T x, T y)           -> void;
            auto center(const Point2<T>& p) -> void;
            auto getCenter() const          -> Vec2<T>;

            auto contains(const Point2<T>& point) const                 -> bool;
            auto intersect(const Point2<T>& point) const                -> bool;
            auto contains(const AABB<T>& rect) const                    -> bool;
            auto intersect(const AABB<T>& rect) const                   -> Intersection<T>;
            auto sweep(const Vec2<T>& vel, const Line2<T>& line) const  -> Intersection<T>;
            auto sweep(const Vec2<T>& vel, AABB<T> box) const           -> Intersection<T>;
            auto sweep(const Vec2<T>& vel, const Polygon<T>& pol) const -> Intersection<T>;

            auto operator!=(const AABB<T>& r) const -> bool;
            auto operator==(const AABB<T>& r) const -> bool;

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
