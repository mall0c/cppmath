#ifndef CPPMATH_POINT2_HPP
#define CPPMATH_POINT2_HPP

#include "Vector2.hpp"

#if (defined(__GNUC__) && __cplusplus < 201103) || (defined(_WIN32) && _MSC_VER<1900)
#define constexpr
#endif

namespace geometry
{
    template <class T>
    class Point2
    {
        public:
            constexpr Point2() : x(0), y(0) {};
            constexpr Point2(const T& x_, const T& y_) : x(x_), y(y_) {};

            constexpr Vector2<T> toVector() const
            {
                return Vector2<T>(x, y);
            }

            template <class T2>
            constexpr operator Point2<T2>() const
            {
                return Point2<T2>(static_cast<T2>(x), static_cast<T2>(y));
            }

            template <class T2>
            Point2<T>& operator+=(const Vector2<T>& vec) const
            {
                x += vec.x;
                y += vec.y;
                return *this;
            }

            template <class T2>
            constexpr Point2<T> operator+(const Vector2<T>& vec) const
            {
                return Point2<T>(x + vec.x, y + vec.y);
            }

            template <class T2>
            constexpr Vector2<T> operator-(const Point2<T>& p2) const
            {
                return Vector2<T>(x - p2.x, y - p2.y);
            }

        public:
            T x, y;
    };
}

#endif
