#ifndef CPPMATH_VECTOR_HPP
#define CPPMATH_VECTOR_HPP

#include "VectorData.hpp"
#include "../math.hpp"
#include <type_traits>

// TODO: Create global functions for dot, signs, abs, ...

namespace geometry
{
    template <typename T, size_t N>
    class Vector : public VectorData<T, N>
    {
        public:
            typedef Vector<T, N> type;

            // Saves writing VectorData<T, N>::_data or this->_data everytime
            using VectorData<T, N>::_data;

        public:
            Vector() : Vector(0) {}
            Vector(const T& val);

            template <typename... Args>
            Vector(const Args&... args);

            // Specific to 2D.
            // Creates a vector of a given length in a given direction.
            // TODO: Write this for 3D
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            static type fromDirection(float length, float dir);

        public:
            // Calls a callback for each element in the vector.
            // The callback takes a pointer to the current element as argument.
            template <typename F>
            void foreach(F callback);

            // Magnitude without sqrt
            double abs_sqr() const;
            double abs() const;

            void normalize();
            type normalized() const;

            T dot(const type& vec) const;

            // Returns a vector with the elements' signs.
            type signs() const;

            template <typename... Args>
            void fill(const Args&... args);
            void fill(const T& val);

            // Wrappers around fill() for compatibility reasons
            template <typename... Args>
            void set(const Args&... args);
            void set(const T& val);

            bool almostEquals(const type& rhs) const;
            bool almostEquals(const type& rhs, T tolerance) const;

        public:
            // For >=3D vectors.
            template <size_t M = N, typename = typename std::enable_if<M >= 3>::type>
            type cross(const type& rhs) const;

            // Returns the magnitude of the 3D cross product with z = 0.
            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            T cross(const type& rhs) const;

            // Produces less rounding errors when checking the cross product for 0
            // Specific to 2D vectors.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            bool crossAlmostZero(const type& rhs) const;

            // Specific to 2D.
            // Returns the direction the vector points in.
            template <size_t M = N, typename = typename std::enable_if<M == 2>::type>
            double dir() const;

        public:
            type operator+(const type& p) const;
            type& operator+=(const type& p);

            type operator-() const;
            type operator-(const type& p) const;
            type& operator-=(const type& p);

            template <typename T2>
            type operator*(const Vector<T2, N>& p) const;

            template <typename T2>
            type operator*(const T2& val) const;

            template <typename T2>
            type& operator*=(const Vector<T2, N>& p);

            template <typename T2>
            type& operator*=(const T2& val);

            template <typename T2>
            type operator/(const Vector<T2, N>& p) const;

            template <typename T2>
            type operator/(const T2& val) const;

            template <typename T2>
            type& operator/=(const Vector<T2, N>& p);

            template <typename T2>
            type& operator/=(const T2& val);

            // These do element wise comparison. They DON'T compare length.
            bool operator<(const type& p) const;
            bool operator>(const type& p) const;
            bool operator<=(const type& p) const;
            bool operator>=(const type& p) const;
            bool operator!=(const type& p) const;
            bool operator==(const type& p) const;

            operator bool() const;

            template <typename T2>
            operator Vector<T2, N>() const;

            const T& operator[](size_t i) const;
            T& operator[](size_t i);

        private:
            template <size_t i, typename U, typename... Args>
            void _fill(const U& val, const Args&... rest);

            template <size_t i, typename U>
            void _fill(const U& val);
    };

    typedef Vector<float, 2> Vec2f;
    typedef Vector<float, 3> Vec3f;
    typedef Vector<int, 2> Vec2i;
    typedef Vector<int, 3> Vec3i;

    template <typename T>
    using Vec2 = Vector<T, 2>;

    template <typename T>
    using Vec3 = Vector<T, 3>;
}

#include "Vector.inl"

#endif
