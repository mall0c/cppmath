#ifndef CPPMATH_VECTOR_INL
#define CPPMATH_VECTOR_INL

#include "Vector.hpp"
#include "Point2.hpp"

#define _FOREACH_VECTOR(var, op) for (size_t (var) = 0; (var) < N; ++(var)) { op }
#define __LIMITDIM_IMPL template <size_t M, typename>

namespace math
{
    template <typename T, size_t N>
    Vector<T, N> mins(const Vector<T, N>& a, const Vector<T, N>& b)
    {
        Vector<T, N> vec;
        _FOREACH_VECTOR(i, vec[i] = std::min(a[i], b[i]);)
        return vec;
    }

    template <typename T, size_t N>
    Vector<T, N> maxs(const Vector<T, N>& a, const Vector<T, N>& b)
    {
        Vector<T, N> vec;
        _FOREACH_VECTOR(i, vec[i] = std::max(a[i], b[i]);)
        return vec;
    }

    template <typename T, size_t N>
    Vector<T, N> abs(const Vector<T, N>& vec)
    {
        Vector<T, N> out;
        _FOREACH_VECTOR(i, out[i] = std::abs(vec[i]);)
        return out;
    }



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
    template <typename First, typename Second, typename... Args>
    Vector<T, N>::Vector(const First& first, const Second& second, const Args&... args)
    {
        fill(first, second, args...);
    }

    template <typename T, size_t N>
    template <typename F>
    void Vector<T, N>::foreach(F callback)
    {
        _FOREACH_VECTOR(i, callback(_data + i, i);)
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
    T Vector<T, N>::project(const type& vec) const
    {
        return dot(vec.normalized());
    }

    template <typename T, size_t N>
    double Vector<T, N>::angle(const type& vec) const
    {
        return radtodeg(angle_rad(vec));
    }

    template <typename T, size_t N>
    double Vector<T, N>::angle_rad(const type& vec) const
    {
        return acos(angle_cos(vec)) * math::sign(this->cross(vec));
    }

    template <typename T, size_t N>
    double Vector<T, N>::angle_cos(const type& vec) const
    {
        return dot(vec) / (abs() * vec.abs());
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    double Vector<T, N>::angle() const
    {
        return radtodeg(angle_rad());
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    double Vector<T, N>::angle_rad() const
    {
        auto rad = -atan2(this->y, this->x);
        return rad >= 0 ? rad : 2 * M_PI + rad;
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    void Vector<T, N>::rotate(float angle)
    {
        rotate_rad(degtorad(angle));
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    void Vector<T, N>::rotate_rad(float rad)
    {
        fill(this->x * cos(rad) - this->y * sin(rad),
             this->x * sin(rad) + this->y * cos(rad));
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    Vector<T, N> Vector<T, N>::rotated(float angle) const
    {
        type v(*this);
        v.rotate(angle);
        return v;
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    Vector<T, N> Vector<T, N>::rotated_rad(float rad) const
    {
        type v(*this);
        v.rotate_rad(rad);
        return v;
    }


    template <typename T, size_t N>
    Vector<T, N> Vector<T, N>::signs() const
    {
        type vec;
        _FOREACH_VECTOR(i, vec[i] = sign(_data[i]);)
        return vec;
    }

    template <typename T, size_t N>
    Vector<T, N>& Vector<T, N>::fill(const T& val)
    {
        _FOREACH_VECTOR(i, _data[i] = val;)
        return *this;
    }

    template <typename T, size_t N>
    template <typename First, typename Second, typename... Args>
    Vector<T, N>& Vector<T, N>::fill(const First& first, const Second& second, const Args&... args)
    {
        _fill<0>(first, second, args...);
        return *this;
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
    bool Vector<T, N>::isZero() const
    {
        return *this == type();
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    const Point2<T>& Vector<T, N>::asPoint() const
    {
        return *reinterpret_cast<const Point2<T>*>(this);
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    Point2<T>& Vector<T, N>::asPoint()
    {
        return *reinterpret_cast<Point2<T>*>(this);
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    T Vector<T, N>::cross(const type& rhs) const
    {
        return rhs.y * this->x - rhs.x * this->y;
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    Vector<T, N> Vector<T, N>::left() const
    {
        return -right();
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    Vector<T, N> Vector<T, N>::right() const
    {
        return type(-this->y, this->x);
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    bool Vector<T, N>::crossAlmostZero(const type& rhs) const
    {
        return math::almostEquals(rhs.y * this->x, rhs.x * this->y);
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    Vector<T, N> Vector<T, N>::cross(const type& rhs) const
    {
        type vec;
        _FOREACH_VECTOR(i,
            vec[i] = _data[i] * rhs[(i+1) % N] - _data[(i+1) % N] * rhs[i];
        )
        return vec;
    }

    template <typename T, size_t N> __LIMITDIM_IMPL
    Vector<T, N> Vector<T, N>::fromAngle(float len, float dir)
    {
        return type(len * cos(degtorad(dir)), len * sin(degtorad(dir)));
    }


    template <typename T, size_t N>
    template <typename T2>
    auto Vector<T, N>::operator+(const other_type<T2>& p) const -> Vector<T, N>::res_type<T2>
    {
        res_type<T2> vec(*this);
        vec += p;
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator+=(const other_type<T2>& p)
    {
        _FOREACH_VECTOR(i, _data[i] += p[i];)
        return *this;
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
    auto Vector<T, N>::operator-(const other_type<T2>& p) const -> Vector<T, N>::res_type<T2>
    {
        res_type<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] -= p[i];)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    Vector<T, N>& Vector<T, N>::operator-=(const other_type<T2>& p)
    {
        _FOREACH_VECTOR(i, _data[i] -= p[i];)
        return *this;
    }

    template <typename T, size_t N>
    template <typename T2>
    auto Vector<T, N>::operator*(const other_type<T2>& p) const -> Vector<T, N>::res_type<T2>
    {
        res_type<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] *= p[i];)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    auto Vector<T, N>::operator*(const T2& val) const -> Vector<T, N>::res_type<T2>
    {
        res_type<T2> vec(*this);
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
    auto Vector<T, N>::operator/(const other_type<T2>& p) const -> Vector<T, N>::res_type<T2>
    {
        res_type<T2> vec(*this);
        _FOREACH_VECTOR(i, vec[i] /= p[i];)
        return vec;
    }

    template <typename T, size_t N>
    template <typename T2>
    auto Vector<T, N>::operator/(const T2& val) const -> Vector<T, N>::res_type<T2>
    {
        res_type<T2> vec(*this);
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
        _FOREACH_VECTOR(i,
            if (_data[i] > p[i])
                return false;
        )
        return true;
    }

    template <typename T, size_t N>
    bool Vector<T, N>::operator>=(const type& p) const
    {
        _FOREACH_VECTOR(i,
            if (_data[i] < p[i])
                return false;
        )
        return true;
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

#undef __LIMITDIM_IMPL

#endif
