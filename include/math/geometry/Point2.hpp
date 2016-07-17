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

            void set(const T& a, const T& b)
            {
                x = a;
                y = b;
            }

            void set(const T& val)
            {
                x = y = val;
            }

            template <class T2>
            constexpr operator Point2<T2>() const
            {
                return Point2<T2>(static_cast<T2>(x), static_cast<T2>(y));
            }

            Point2<T>& operator-=(const Vector2<T>& vec) const
            {
                x -= vec.x;
                y -= vec.y;
                return *this;
            }

            constexpr Point2<T> operator-(const Vector2<T>& vec) const
            {
                return Point2<T>(x - vec.x, y - vec.y);
            }

            Point2<T>& operator+=(const Vector2<T>& vec) const
            {
                x += vec.x;
                y += vec.y;
                return *this;
            }

            constexpr Point2<T> operator+(const Vector2<T>& vec) const
            {
                return Point2<T>(x + vec.x, y + vec.y);
            }

            constexpr Vector2<T> operator-(const Point2<T>& p2) const
            {
                return Vector2<T>(x - p2.x, y - p2.y);
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
    typedef Point2<int> Point2i;
}

#endif
