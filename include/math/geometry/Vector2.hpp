#ifndef CPPMATH_VECTOR_2_HPP
#define CPPMATH_VECTOR_2_HPP

#include <math.h>

#if (defined(__GNUC__) && __cplusplus < 201103) || (defined(_WIN32) && _MSC_VER<1900)
#define constexpr
#endif

namespace geometry
{
    template <class T>
    class Vector2
    {
        public:
            constexpr Vector2() : Vector2(0, 0) {}
            constexpr Vector2(const Vector2<T>& vec) : Vector2(vec.x, vec.y) {}
            constexpr Vector2(const T& x_, const T& y_) : x(x_), y(y_) {}

        public:
            constexpr double abs() const
            {
                return sqrt(x * x + y * y);
            }

            void normalize()
            {
                *this /= abs();
            }

            constexpr T scalar(const Vector2<T>& vec) const
            {
                return x * vec.x + y * vec.y;
            }

            constexpr double dir() const
            {
                //angle between 2 vectors: scalar(vec1, vec2) / abs(vec1) * abs(vec2)
                //-> direction = angle between vec(x, y) and the x-axis vec(1, 0)
                return acos(x / abs()) / M_PI * 180;
            }


            void set(const T& x_, const T& y_)
            {
                x = x_;
                y = y_;
            }

            void set(const T& val)
            {
                x = y = val;
            }


            template <class T2>
            constexpr Vector2<T> operator+(const Vector2<T2>& p) const
            {
                return Vector2<T>(x + p.x, y + p.y);
            }

            template <class T2>
            Vector2<T>& operator+=(const Vector2<T2>& p)
            {
                return *this = *this + p;
            }


            constexpr Vector2<T> operator-() const
            {
                return Vector2<T>(-x, -y);
            }

            template <class T2>
            constexpr Vector2<T> operator-(const Vector2<T2>& p) const
            {
                return Vector2<T>(x - p.x, y - p.y);
            }

            template <class T2>
            Vector2<T>& operator-=(const Vector2<T2>& p)
            {
                return *this = *this - p;
            }


            //This is NOT scalar multiplication, use scalar() instead
            template <class T2>
            constexpr Vector2<T> operator*(const Vector2<T2>& p) const
            {
                return Vector2<T>(x * p.x, y * p.y);
            }

            template <class T2>
            constexpr Vector2<T> operator*(const T2& val) const
            {
                return Vector2<T>(x * val, y * val);
            }

            template <class T2>
            Vector2<T>& operator*=(const Vector2<T2>& p)
            {
                return *this = *this * p;
            }

            template <class T2>
            Vector2<T>& operator*=(const T2& val)
            {
                return *this = *this * val;
            }


            template <class T2>
            constexpr Vector2<T> operator/(const T2& val) const
            {
                return Vector2<T>(x / val, y / val);
            }

            template <class T2>
            constexpr Vector2<T> operator/(const Vector2<T2>& p) const
            {
                return Vector2<T>(x / p.x, y / p.y);
            }

            template <class T2>
            Vector2<T>& operator/=(const Vector2<T2>& p)
            {
                return *this = *this / p;
            }

            template <class T2>
            Vector2<T>& operator/=(const T2& val)
            {
                return *this = *this / val;
            }


            constexpr bool operator<(const Vector2<T>& p) const
            {
                return x < p.x && y < p.y;
            }

            constexpr bool operator>(const Vector2<T>& p) const
            {
                return x > p.x && y > p.y;
            }

            constexpr bool operator<=(const Vector2<T>& p) const
            {
                return x <= p.x && y <= p.y;
            }

            constexpr bool operator>=(const Vector2<T>& p) const
            {
                return x >= p.x && y >= p.y;
            }

            constexpr bool operator!=(const Vector2<T>& p) const
            {
                return x != p.x || y != p.y;
            }

            constexpr bool operator==(const Vector2<T>& p) const
            {
                return x == p.x && y == p.y;
            }

            constexpr operator bool() const
            {
                return x || y;
            }


            template <class T2>
            constexpr operator Vector2<T2>() const
            {
                return Vector2<T2>(static_cast<T2>(x), static_cast<T2>(y));
            }


        public:
            T x, y;

    };

    template <class T>
    Vector2<T> fromDirection(float length, float dir)
    {
        return Vector2<T>(length * cos(dir * M_PI / 180), length * sin(dir * M_PI / 180));
    }
}

#endif

