#ifndef CPPMATH_VECTOR_HPP
#define CPPMATH_VECTOR_HPP

#include "VectorData.hpp"
#include "../math.hpp"
#include "../type_traits.hpp"

// TODO: Create global functions for dot, signs, abs, ...

namespace math
{
    template <typename T>
    class Point2;

    template <typename T, size_t N>
    class Vector : public VectorData<T, N>
    {
        public:
            typedef Vector<T, N> type;

            template <typename T2>
            using other_type = Vector<T2, N>;

            template <typename T2>
            using res_type = Vector<typename math::common_type<T, T2>::type, N>;

            // Saves writing VectorData<T, N>::_data or this->_data everytime
            using VectorData<T, N>::_data;

        public:
            Vector() : Vector(0) {}

            explicit Vector(const T& val);

            template <typename T2>
            explicit Vector(const other_type<T2>& vec);

            template <typename First, typename Second, typename... Args>
            explicit Vector(const First& first, const Second& second, const Args&... args);

            // Creates a vector of a given length in a given direction.
            // Specific to 2D vectors.
            // TODO: Write this for 3D
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            static type fromAngle(float length, float dir);

        public:
            // Calls a callback for each element in the vector.
            // The callback takes a pointer to the current element as argument.
            template <typename F>
            void foreach(F callback);

            constexpr size_t size() const
            {
                return N;
            }

            // (Squared) Magnitude of the vector.
            double abs() const;
            double abs_sqr() const;

            void normalize();
            type normalized() const;

            T dot(const type& vec) const;
            T project(const type& vec) const;

            // Returns the angle between this and another vector in
            // degrees/radians/cosines.
            double angle(const type& vec) const;
            double angle_rad(const type& vec) const;
            double angle_cos(const type& vec) const;

            // Returns the direction of the vector in degrees/radians/cosines.
            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            double angle() const;

            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            double angle_rad() const;

            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            double angle_cos() const;

            // Returns a vector with the elements' signs.
            type signs() const;

            template <typename First, typename Second, typename... Args>
            type& fill(const First& first, const Second& second, const Args&... args);
            type& fill(const T& val);

            bool almostEquals(const type& rhs) const;
            bool almostEquals(const type& rhs, T tolerance) const;

            // Specific to 3D and 7D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 3 || M == 7>::type>
            type cross(const type& rhs) const;

            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            const Point2<T>& asPoint() const;

            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            Point2<T>& asPoint();

            // Returns a vector perpendicular to this vector.
            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            type ortho() const;

            // Returns the magnitude of the 3D cross product with z = 0.
            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            T cross(const type& rhs) const;

            // Produces less rounding errors when checking the cross product for 0
            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            bool crossAlmostZero(const type& rhs) const;

        public:
            template <typename T2>
            res_type<T2> operator+(const other_type<T2>& p) const;

            template <typename T2>
            type& operator+=(const other_type<T2>& p);

            type operator-() const;

            template <typename T2>
            res_type<T2> operator-(const other_type<T2>& p) const;

            template <typename T2>
            type& operator-=(const other_type<T2>& p);

            template <typename T2>
            res_type<T2> operator*(const other_type<T2>& p) const;

            template <typename T2>
            res_type<T2> operator*(const T2& val) const;

            template <typename T2>
            type& operator*=(const other_type<T2>& p);

            template <typename T2>
            type& operator*=(const T2& val);

            template <typename T2>
            res_type<T2> operator/(const other_type<T2>& p) const;

            template <typename T2>
            res_type<T2> operator/(const T2& val) const;

            template <typename T2>
            type& operator/=(const other_type<T2>& p);

            template <typename T2>
            type& operator/=(const T2& val);

            // These do element wise comparison. They DON'T compare length.
            bool operator<(const type& p) const;
            bool operator>(const type& p) const;
            bool operator<=(const type& p) const;
            bool operator>=(const type& p) const;
            bool operator!=(const type& p) const;
            bool operator==(const type& p) const;

            // operator bool() const;

            template <typename T2>
            operator other_type<T2>() const;

            const T& operator[](size_t i) const;
            T& operator[](size_t i);

        private:
            template <size_t i, typename U, typename... Args>
            void _fill(const U& val, const Args&... rest);

            template <size_t i, typename U>
            void _fill(const U& val);
    };

    template <typename T, size_t N>
    auto min(const Vector<T, N>& a, const Vector<T, N>& b) -> Vector<T, N>;

    template <typename T, size_t N>
    auto max(const Vector<T, N>& a, const Vector<T, N>& b) -> Vector<T, N>;

    typedef Vector<float, 2> Vec2f;
    typedef Vector<float, 3> Vec3f;
    typedef Vector<double, 2> Vec2d;
    typedef Vector<double, 3> Vec3d;
    typedef Vector<int, 2> Vec2i;
    typedef Vector<int, 3> Vec3i;

    template <typename T>
    using Vec2 = Vector<T, 2>;

    template <typename T>
    using Vec3 = Vector<T, 3>;
}

#include "Vector.inl"

#endif
