#ifndef CPPMATH_POINT2_HPP
#define CPPMATH_POINT2_HPP

#include "Vector.hpp"
#include "../compat.hpp"

namespace math
{
    template <typename T>
    class Point2
    {
        public:
            constexpr Point2() : x(0), y(0) {};
            constexpr Point2(const T& x_, const T& y_) : x(x_), y(y_) {};

            Vec2<T> toVector() const
            {
                return Vec2<T>(x, y);
            }

            const Vec2<T>& asVector() const
            {
                return *reinterpret_cast<const Vec2<T>*>(this);
            }

            Vec2<T>& asVector()
            {
                return *reinterpret_cast<Vec2<T>*>(this);
            }

            void set(const T& a, const T& b)
            {
                x = a;
                y = b;
            }

            void set(const T& val)
            {
                x = y = val;
            }

            constexpr bool operator!=(const Point2<T>& p) const
            {
                return !operator==(p);
            }

            constexpr bool operator==(const Point2<T>& p) const
            {
                return x == p.x && y == p.y;
            }

            template <typename T2>
            constexpr operator Point2<T2>() const
            {
                return Point2<T2>(static_cast<T2>(x), static_cast<T2>(y));
            }

            template <typename T2>
            Point2<T>& operator-=(const Vec2<T2>& vec)
            {
                x -= vec.x;
                y -= vec.y;
                return *this;
            }

            template <typename T2>
            Point2<T> operator-(const Vec2<T2>& vec) const
            {
                return Point2<T>(x - vec.x, y - vec.y);
            }

            template <typename T2>
            Point2<T>& operator+=(const Vec2<T2>& vec)
            {
                x += vec.x;
                y += vec.y;
                return *this;
            }

            template <typename T2>
            Point2<T> operator+(const Vec2<T2>& vec) const
            {
                return Point2<T>(x + vec.x, y + vec.y);
            }

            template <typename T2>
            Vec2<T> operator-(const Point2<T2>& p2) const
            {
                return Vec2<T>(x - p2.x, y - p2.y);
            }

            constexpr const T& operator[](size_t i) const
            {
                return data[i];
            }

            T& operator[](size_t i)
            {
                return data[i];
            }

        public:
            union {
                struct {
                    T x, y;
                };
                T data[2];
            };
    };

    typedef Point2<float> Point2f;
    typedef Point2<double> Point2d;
    typedef Point2<int> Point2i;

    template <typename T, typename T2>
    constexpr Point2<T> snap(const Point2<T>& p, const Point2<T2> gridsize)
    {
        return Point2<T>(snap(p.x, gridsize.x), snap(p.y, gridsize.y));
    }

    template <typename T, typename T2>
    constexpr Point2<T> smartSnap(const Point2<T>& p, const Point2<T2> gridsize)
    {
        return Point2<T>(smartSnap(p.x, gridsize.x), smartSnap(p.y, gridsize.y));
    }
}

#endif
