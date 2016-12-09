#ifndef CPPMATH_VECTOR_INL
#define CPPMATH_VECTOR_INL

#include "Vector.hpp"
#include "../TypeTraits.hpp" // almostEquals

#define _FOREACH_VECTOR(var, op) for (size_t (var) = 0; (var) < N; ++(var)) { op }

namespace geometry
{
    template <typename T, size_t N>
    Vector<T, N>::Vector(const T& val)
    {
        fill(val);
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::Vector(const other_type<T2>& vec)
    {
        _FOREACH_VECTOR(i, _data[i] = static_cast<T>(vec[i]);)
    }

    template <typename T, size_t N>
    template <typename First, typename... Args>
    Vector<T, N>::Vector(const First& first, const Args&... args)
    {
        fill(first, args...);
    }

    template <typename T, size_t N>
    template <typename F>
    void Vector<T, N>::foreach(F callback)
    {
        _FOREACH_VECTOR(i, callback(_data + i);)
    }

    template <typename T, size_t N>
    double Vector<T, N>::abs() const
    {
        return sqrt(dot(*this));
    }

    template <typename T, size_t N>
    double Vector<T, N>::abs_sqr() const
    {
        return dot(*this);
    }

    template <typename T, size_t N>
    void Vector<T, N>::normalize()
    {
        *this /= abs();
    }

    template <typename T, size_t N>
    Vector<T, N> Vector<T, N>::normalized() const
    {
        return *this / abs();
    }

    template <typename T, size_t N>
    T Vector<T, N>::dot(const type& vec) const
    {
        T tmp = 0;
        _FOREACH_VECTOR(i, tmp += _data[i] * vec[i];)
        return tmp;
    }

    template <typename T, size_t N>
    Vector<T, N> Vector<T, N>::signs() const
    {
        type vec;
        _FOREACH_VECTOR(i, vec[i] = math::sign(_data[i]);)
        return vec;
    }

    template <typename T, size_t N>
    void Vector<T, N>::fill(const T& val)
    {
        _FOREACH_VECTOR(i, _data[i] = val;)
    }

    template <typename T, size_t N>
    template <typename... Args>
    void Vector<T, N>::fill(const Args&... args)
    {
        _fill<0>(args...);
    }

    template <typename T, size_t N>
    void Vector<T, N>::set(const T& val)
    {
        fill(val);
    }

    template <typename T, size_t N>
    template <typename... Args>
    void Vector<T, N>::set(const Args&... args)
    {
        fill(args...);
    }

    template <typename T, size_t N>
    bool Vector<T, N>::almostEquals(const type& p) const
    {
        _FOREACH_VECTOR(i,
            if (!math::almostEquals(_data[i], p[i]))
                return false;
        )
        return true;
    }

    template <typename T, size_t N>
    bool Vector<T, N>::almostEquals(const type& p, T tolerance) const
    {
        _FOREACH_VECTOR(i,
            if (!math::almostEquals(_data[i], p[i], tolerance))
                return false;
        )
    }


    template <typename T, size_t N>
    template <size_t M, typename>
    T Vector<T, N>::cross(const type& rhs) const
    {
        return rhs.y * this->x - rhs.x * this->y;
    }

    template <typename T, size_t N>
    template <size_t M, typename>
    bool Vector<T, N>::crossAlmostZero(const type& rhs) const
    {
        return math::almostEquals(rhs.y * this->x, rhs.x * this->y);
    }

    template <typename T, size_t N>
    template <size_t M, typename>
    Vector<T, N> Vector<T, N>::cross(const type& rhs) const
    {
        type vec;
        _FOREACH_VECTOR(i,
            vec[i] = _data[i] * rhs[(i+1) % N] - _data[(i+1) % N] * rhs[i];
        )
        return vec;
    }

    template <typename T, size_t N>
    template <size_t M, typename>
    double Vector<T, N>::dir() const
    {
        //angle between 2 vectors: dot(vec1, vec2) / abs(vec1) * abs(vec2)
        //-> direction = angle between vec(x, y) and the x-axis vec(1, 0)
        return acos(this->x / abs()) / M_PI * 180;
    }

    template <typename T, size_t N>
    template <size_t M, typename>
    Vector<T, N> Vector<T, N>::fromDirection(float len, float dir)
    {
        return type(len * cos(dir * M_PI / 180), len * sin(dir * M_PI / 180));
    }


    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::ResVec<T2> Vector<T, N>::operator+(const other_type<T2>& p) const
    {
        ResVec<T2> vec(*this);
        vec += p;
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator+=(const other_type<T2>& p)
    {
        _FOREACH_VECTOR(i, _data[i] += p[i];)
    }

    template <typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator-() const
    {
        type vec(*this);
        _FOREACH_VECTOR(i, vec[i] = -vec[i];)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::ResVec<T2> Vector<T, N>::operator-(const other_type<T2>& p) const
    {
        ResVec<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] - p[i];)
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator-=(const other_type<T2>& p)
    {
        _FOREACH_VECTOR(i, _data[i] -= p[i];)
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::ResVec<T2> Vector<T, N>::operator*(const other_type<T2>& p) const
    {
        ResVec<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] *= p[i];)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::ResVec<T2> Vector<T, N>::operator*(const T2& val) const
    {
        ResVec<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] *= val;)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator*=(const Vector<T2, N>& p)
    {
        _FOREACH_VECTOR(i, _data[i] *= p[i];)
        return *this;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator*=(const T2& val)
    {
        _FOREACH_VECTOR(i, _data[i] *= val;)
        return *this;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::ResVec<T2> Vector<T, N>::operator/(const other_type<T2>& p) const
    {
        ResVec<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] /= p[i];)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::ResVec<T2> Vector<T, N>::operator/(const T2& val) const
    {
        ResVec<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] /= val;)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator/=(const Vector<T2, N>& p)
    {
        _FOREACH_VECTOR(i, _data[i] /= p[i];)
        return *this;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator/=(const T2& val)
    {
        _FOREACH_VECTOR(i, _data[i] /= val;)
        return *this;
    }

    template <typename T, size_t N>
    bool Vector<T, N>::operator<(const type& p) const
    {
        _FOREACH_VECTOR(i,
            if (_data[i] >= p[i])
                return false;
        )
        return true;
    }

    template <typename T, size_t N>
    bool Vector<T, N>::operator>(const type& p) const
    {
        _FOREACH_VECTOR(i,
            if (_data[i] <= p[i])
                return false;
        )
        return true;
    }

    template <typename T, size_t N>
    bool Vector<T, N>::operator<=(const type& p) const
    {
        return !(*this > p);
    }

    template <typename T, size_t N>
    bool Vector<T, N>::operator>=(const type& p) const
    {
        return !(*this < p);
    }

    template <typename T, size_t N>
    bool Vector<T, N>::operator!=(const type& p) const
    {
        _FOREACH_VECTOR(i,
            if (_data[i] != p[i])
                return true;
        )
        return false;
    }

    template <typename T, size_t N>
    bool Vector<T, N>::operator==(const type& p) const
    {
        return !(*this != p);
    }

    // template <typename T, size_t N>
    // Vector<T, N>::operator bool() const
    // {
    //     assert(false);
    //     _FOREACH_VECTOR(i,
    //         if (_data[i])
    //             return true;
    //     )
    //     return false;
    // }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>::operator Vector<T2, N>() const
    {
        Vector<T2, N> vec;
        _FOREACH_VECTOR(i, vec[i] = static_cast<T2>(_data[i]);)
        return vec;
    }

    template <typename T, size_t N>
    const T& Vector<T, N>::operator[](size_t i) const
    {
        return VectorData<T, N>::_data[i];
    }

    template <typename T, size_t N>
    T& Vector<T, N>::operator[](size_t i)
    {
        return _data[i];
    }

    template <typename T, size_t N>
    template <size_t i, typename U, typename... Args>
    void Vector<T, N>::_fill(const U& val, const Args&... rest)
    {
        _data[i] = val;
        _fill<i + 1>(rest...);
    }

    template <typename T, size_t N>
    template <size_t i, typename U>
    void Vector<T, N>::_fill(const U& val)
    {
        _data[i] = val;
    }
}

#endif
