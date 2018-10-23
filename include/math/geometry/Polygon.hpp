#ifndef CPPMATH_POLYGON_HPP
#define CPPMATH_POLYGON_HPP

#include <vector>
#include "Line2.hpp"

namespace math
{
    enum PolygonType
    {
        LineStrip,
        TriangleStrip,
    };

    template <typename T>
    class BasePolygon
    {
        public:
            BasePolygon(PolygonType type = TriangleStrip, NormalDirection ndir = NormalBoth);
            BasePolygon(size_t size, PolygonType type = TriangleStrip, NormalDirection ndir = NormalBoth);
            virtual ~BasePolygon() {};

            // Add or get/edit the i-th vertex with applied transformation.
            // Negative indices wrap around the end, e.g. -1 -> size() - 1
            virtual auto add(const Point2<T>& point)        -> void;
            virtual auto edit(size_t i, const Point2<T>& p) -> void;
            virtual auto get(int i) const                   -> Point2<T>;

            // Add or get/edit the i-th vertex, disregarding transformations.
            // Negative indices wrap around the end, e.g. -1 -> size() - 1
            auto addRaw(const Point2<T>& point)        -> void;
            auto editRaw(size_t i, const Point2<T>& p) -> void;
            auto getRaw(int i) const                   -> const Point2<T>&;

            // Return a line segment from point i to point j.
            // Negative indices wrap around the end, e.g. -1 -> size() - 1
            auto getSegment(int i, int j, bool raw = false) const -> Line2<T>;

            // Callback signature: (const Line2<T>&) -> bool
            // Returning true breaks the loop.
            template <typename F>
            auto foreachSegment(F callback, bool raw = false) const -> void;

            auto intersect(const Line2<T>& line, bool convex = false, bool invert = false) const   -> Intersection<T>;
            auto intersect(const Point2<T>& point, bool convex = false, bool invert = false) const -> bool;

            // Returns nearest intersection
            // Basically the same as intersect(), but without early-out checks.
            // Is used internally by intersect().
            auto findNearest(const Line2<T>& line) const -> Intersection<T>;

            // Returns the bounding box with offset applied
            auto getBBox() const -> const AABB<T>&;

            // Remove all vertices
            auto clear() -> void;

            auto size() const -> size_t;

        public:
            PolygonType type;
            NormalDirection normaldir;

        protected:
            // Called whenever the vertix list changed
            virtual auto _onVertexChanged() -> void {};

            // Recalculate the bounding box
            auto _recalculate() -> void;

            // non-const getRaw() version
            auto _getRaw(int i) -> Point2<T>&;

        protected:
            AABB<T> _bbox;
            std::vector<Point2<T>> _points;
    };


    template <typename T>
    class Polygon : public BasePolygon<T>
    {
        public:
            Polygon(PolygonType type = TriangleStrip, NormalDirection ndir = NormalBoth);
            Polygon(size_t size, PolygonType type = TriangleStrip, NormalDirection ndir = NormalBoth);
            virtual ~Polygon() {};

            virtual auto add(const Point2<T>& point)        -> void;
            virtual auto edit(size_t i, const Point2<T>& p) -> void;
            virtual auto get(int i) const                   -> Point2<T>;

            auto setOffset(const Vec2<T>& off)  -> void;
            auto getOffset() const              -> const Vec2<T>&;
            auto move(const Vec2<T>& rel)       -> void;
            auto setScale(const Vec2<T>& scale) -> void;
            auto getScale() const               -> const Vec2<T>&;

        protected:
            Vec2<T> _offset;
            Vec2<T> _scale;
    };
}

#include "Polygon.inl"

#endif
