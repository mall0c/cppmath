#ifndef MATH_MESH_INTERSECT_FUNCTIONS_HPP
#define MATH_MESH_INTERSECT_FUNCTIONS_HPP

#include "intersect.hpp"

namespace math
{
    template <typename T> bool intersectTriangles(const Point2<T>& point, const AbstractPointSet<T>& mesh);
    template <typename T> bool intersectTriangleStrip(const Point2<T>& point, const AbstractPointSet<T>& mesh);
    template <typename T> bool intersectTriangleFan(const Point2<T>& point, const AbstractPointSet<T>& mesh);
    template <typename T> bool intersectQuads(const Point2<T>& point, const AbstractPointSet<T>& mesh);
}


// Implementation
namespace math
{
#define PRE_CHECK_BOUNDS(minsize, p, mesh) \
        if (mesh.size() < (minsize))   \
            return false;   \
    \
        if (!intersect((mesh).getBBox(), (p)))  \
            return false;


    template <typename T>
    bool intersectTriangles(const Point2<T>& point, const AbstractPointSet<T>& mesh)
    {
        PRE_CHECK_BOUNDS(3, point, mesh);

        for (size_t i = 2; i < mesh.size(); i += 3)
            if (intersect(point,
                        mesh.get(i - 2),
                        mesh.get(i - 1),
                        mesh.get(i)))
                return true;
        return false;
    }


    template <typename T>
    bool intersectTriangleStrip(const Point2<T>& point, const AbstractPointSet<T>& mesh)
    {
        PRE_CHECK_BOUNDS(3, point, mesh);

        for (size_t i = 2; i < mesh.size(); ++i)
            if (intersect(point,
                        mesh.get(i - 2),
                        mesh.get(i - 1),
                        mesh.get(i)))
                return true;
        return false;
    }


    template <typename T>
    bool intersectTriangleFan(const Point2<T>& point, const AbstractPointSet<T>& mesh)
    {
        PRE_CHECK_BOUNDS(3, point, mesh);

        for (size_t i = 2; i < mesh.size(); ++i)
            if (intersect(point,
                        mesh.get(0),
                        mesh.get(i - 1),
                        mesh.get(i)))
                return true;
        return false;
    }


    template <typename T>
    bool intersectQuads(const Point2<T>& point, const AbstractPointSet<T>& mesh)
    {
        PRE_CHECK_BOUNDS(4, point, mesh);

        for (size_t i = 3; i < mesh.size(); i += 4)
            if (intersect(point,
                        mesh.get(i - 3),
                        mesh.get(i - 2),
                        mesh.get(i - 1)) ||
                    intersect(point,
                        mesh.get(i - 3),
                        mesh.get(i - 1),
                        mesh.get(i)))
                return true;
        return false;
    }

#undef PRE_CHECK_BOUNDS
}

#endif
