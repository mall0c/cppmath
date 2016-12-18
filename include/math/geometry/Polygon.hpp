#ifndef CPPMATH_POLYGON_HPP
#define CPPMATH_POLYGON_HPP

#include <vector>
#include "Line2.hpp"

namespace math
{
    template <typename T>
    class Polygon
    {
        public:
            Polygon() {};
            Polygon(size_t size);

            // Add a new vertex
            auto add(const Point2<T>& point) -> void;

            // Edit the i-th vertex
            auto edit(size_t i, const Point2<T>& p) -> void;

            // Return the i-th vertex with applied offset.
            // Negative indices wrap around the end, e.g. -1 -> size() - 1
            auto get(int i) const -> Point2<T>;

            // Return the i-th vertex without offset.
            // Negative indices wrap around the end, e.g. -1 -> size() - 1
            auto getRaw(int i) const -> const Point2<T>&;

            auto setOffset(T x, T y) -> void;
            auto move(T x, T y) -> void;

            // Returns the bounding box with offset applied
            auto getBBox() const -> const AABB<T>&;

            auto size() const -> size_t;

        protected:
            // Recalculate the bounding box
            auto _recalculate() -> void;

            // Return a line segment from point i to point j.
            // Negative indices wrap around the end, e.g. -1 -> size() - 1
            auto _getSegment(int i, int j) const -> Line2<T>;

        protected:
            Vec2<T> _offset;
            AABB<T> _bbox;
            std::vector<Point2<T>> _points;
    };
}

#include "Polygon.inl"

#endif
