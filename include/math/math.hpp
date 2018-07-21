#ifndef CPPMATH_MATH_HPP
#define CPPMATH_MATH_HPP

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "compat.hpp"

namespace math
{
    constexpr const double pi = M_PI;

    double pointDistance(double x1, double y1, double x2, double y2);
    double pointDirection(double x1, double y1, double x2, double y2);

    double lengthdirX(double len, double dir);
    double lengthdirY(double len, double dir);

    double adaptDirection(double dir);

    template <typename T, typename T2>
    constexpr T snap(T x, T2 gridsize)
    {
        return (int)(x / gridsize) * gridsize - (x < 0 ? gridsize : 0);
    }

    template <typename T, typename T2>
    constexpr T smartSnap(T x, T2 gridsize)
    {
        return round(x / gridsize) * gridsize - (x < 0 ? gridsize : 0);
    }

    constexpr double radtodeg(double rad)
    {
        return rad * 180 / pi;
    }

    constexpr double degtorad(double deg)
    {
        return deg * pi / 180;
    }

    template <typename T>
    constexpr int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T>
    T clamp(const T& val, const T& min, const T& max)
    {
        return std::max(min, std::min(val, max));
    }

    template <typename T>
    constexpr bool around(const T& val, const T& base, const T& rad)
    {
        return inrange(val, base - rad, base + rad);
    }

    template <typename T>
    constexpr bool inrange(const T& val, const T& a, const T& b, bool incl = false)
    {
        return incl ? val >= std::min(a, b) && val <= std::max(a, b)
                    : val > std::min(a, b) && val < std::max(a, b);
    }
}

#endif

