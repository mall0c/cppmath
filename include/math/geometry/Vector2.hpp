#ifndef CPPMATH_VECTOR_2_HPP
#define CPPMATH_VECTOR_2_HPP

#include "../math.hpp"
#include "../TypeTraits.hpp"
#include "../compat.hpp"

namespace geometry
{
    template <class T>
    class Vector2
    {
        public:
            constexpr Vector2() : Vector2(0, 0) {}
            constexpr Vector2(const T& x_, const T& y_) : x(x_), y(y_) {}

            constexpr static Vector2<T> fromDirection(float length, float dir)
            {
                return Vector2<T>(length * cos(dir * M_PI / 180),
                        length * sin(dir * M_PI / 180));
            }

        public:
            constexpr double abs() const
            {
                return sqrt(x * x + y * y);
            }

            // Magnitude without sqrt
            constexpr double abs_sqr() const
            {
                return x * x + y * y;
            }

            void normalize()
            {
                *this /= abs();
            }

            Vector2<T> normalized()
            {
                return *this / abs();
            }

            constexpr T scalar(const Vector2<T>& vec) const
            {
                return x * vec.x + y * vec.y;
            }

            // Returns the magnitude of the resulting 3D vector of a
            // 3D cross product with z = 0.
            constexpr T cross(const Vector2<T>& vec) const
            {
                return vec.y * x - vec.x * y;
            }

            // Produces less rounding errors when checking the cross product for 0
            constexpr inline bool crossAlmostZero(const Vector2<T>& vec) const
            {
                return math::almostEquals(vec.y * x, vec.x * y);
            }


            // Returns a vector with the elements' signs.
            constexpr Vector2<T> signs() const
            {
                return Vector2<T>(math::sign(x), math::sign(y));
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

            // Useful for floating point number comparison
            bool almostEquals(const Vector2<T>& p) const
            {
                return math::almostEquals(x, p.x) && math::almostEquals(y, p.y);
            }

            bool almostEquals(const Vector2<T>& p, T tolerance) const
            {
                return math::almostEquals(x, p.x, tolerance) && math::almostEquals(y, p.y, tolerance);
            }

            constexpr Vector2<T> operator+(const Vector2<T>& p) const
            {
                return Vector2<T>(x + p.x, y + p.y);
            }

            Vector2<T>& operator+=(const Vector2<T>& p)
            {
                return *this = *this + p;
            }


            constexpr Vector2<T> operator-() const
            {
                return Vector2<T>(-x, -y);
            }

            constexpr Vector2<T> operator-(const Vector2<T>& p) const
            {
                return Vector2<T>(x - p.x, y - p.y);
            }

            Vector2<T>& operator-=(const Vector2<T>& p)
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

    typedef Vector2<float> Vector2f;
    typedef Vector2<int> Vector2i;
}

#endif

