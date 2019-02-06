#ifndef MATH_INTERSECTION_HPP
#define MATH_INTERSECTION_HPP

#include "Polygon.hpp"

namespace math
{
    // Returns nearest intersection
    // Basically the same as intersect(), but without early-out checks.
    // Is used internally by intersect().
    template <typename T>
    Intersection<T> findNearest(const Line2<T>& line, const AbstractPolygon<T>& pol);


    template <typename T> Intersection<T> intersect(const Line2<T>& line, const AbstractPolygon<T>& pol);
    template <typename T> bool            intersect(const Point2<T>& point, const AbstractPolygon<T>& pol);

    template <typename T> Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, AABB<T> other);
    template <typename T> Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, const AbstractPolygon<T>& pol, bool avgCorners = true, bool backfaceCulling = true);
    template <typename T> Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, const Line2<T>& line, NormalDirection ndir = NormalBoth);
}

// Implementation
namespace math
{
    // Returns nearest intersection
    // Basically the same as intersect(), but without early-out checks.
    // Is used internally by intersect().
    // TODO: convex optimization
    template <typename T>
    Intersection<T> findNearest(const Line2<T>& line, const AbstractPolygon<T>& pol)
    {
        Intersection<T> nearest;

        auto cb = [&](const Line2<T>& seg) {
            auto isec = line.intersect(seg, pol.normaldir);
            if (!nearest || (isec && isec.time < nearest.time))
                nearest = isec;
            return false;
        };

        pol.foreachSegment(cb);
        return nearest;
    }


    template <typename T>
    Intersection<T> intersect(const Line2<T>& line, const AbstractPolygon<T>& pol)
    {
        if (pol.size() < 2)
            return Intersection<T>();

        if (!line.intersect(pol.getBBox()))
            return Intersection<T>();

        auto isec = findNearest(line, pol);
        if (isec)
            return isec;

        // Segment fully inside
        if (pol.closed && pol.filled && line.type == Segment && intersect(line.p, pol))
            return Intersection<T>(line.p, Vec2f(), Vec2f());

        return Intersection<T>();
    }


    template <typename T>
    bool intersect(const Point2<T>& point, const AbstractPolygon<T>& pol)
    {
        if (pol.size() < 2)
            return false;

        if (pol.size() == 2)
            return pol.getSegment(0, 1).intersect(point);

        if (!pol.getBBox().contains(point))
            return false;

        if (pol.closed && pol.filled)
        {
            Line2<T> ray(point, Vec2<T>(1, 0), Ray);
            size_t num = 0;

            pol.foreachSegment([&](const Line2<T>& line) {
                if (ray.intersect(line))
                    ++num;
                return pol.isConvex() ? num == 2 : false;
            });

            return num % 2 == 1;
        }
        else
        {
            for (size_t i = 1; i < pol.size(); ++i)
                if (pol.getSegment(i - 1, i).intersect(point))
                    return true;
        }

        return false;
    }

    template <typename T>
    Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, AABB<T> other)
    {
        other.extend(aabb);
        auto isec = Line2<T>(aabb.getCenter(), vel, Segment).intersect(other);
        if (isec)
            isec.type = SweptAABBxAABB;
        return isec;
    }

    template <typename T>
    Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, const AbstractPolygon<T>& pol, bool avgCorners, bool backfaceCulling)
    {
        if (!sweep(aabb, vel, pol.getBBox()))
            return Intersection<T>();

        Intersection<T> nearest;
        auto cb = [&](const Line2<T>& seg) {
            auto isec = sweep(aabb, vel, seg, pol.normaldir);
            if (isec)
            {
                if (backfaceCulling && isec.normal.dot(vel) > 0)
                    return false;

                if (avgCorners && nearest && std::abs(isec.time - nearest.time) < 0.01)
                {
                    nearest.p = (isec.time < nearest.time) ? isec.p : nearest.p;
                    nearest.time = std::min(nearest.time, isec.time);
                    nearest.normal = (nearest.normal + isec.normal).normalized();
                }
                else if (!nearest || isec.time < nearest.time)
                    nearest = isec;
            }
            return false;
        };
        pol.foreachSegment(cb);

        // TODO: add support for filled polygons

        return nearest;
    }

    template <typename T>
    Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, const Line2<T>& line, NormalDirection ndir)
    {
        // Based on https://gamedev.stackexchange.com/questions/29479/swept-aabb-vs-line-segment-2d
        // Praise OP

        // TODO: support rays
        assert(line.type != Ray && "Rays are not supported yet");

        auto nd = line.d.normalized(); // normalized line direction
        auto half = aabb.size / 2;
        auto dist = nd.left().dot(line.p - aabb.getCenter());
        auto ln = dist < 0 ? nd.right() : nd.left();
        dist = std::abs(dist);
        auto r = half.dot(abs(ln));
        auto velproj = ln.dot(vel);

        if (velproj < 0)
            r *= -1;

        Vec2<T> times(std::max((dist - r) / velproj, 0.0f),
                      std::min((dist + r) / velproj, 1.0f));

        if (line.type == Segment)
        {
            // AABB vs AABB sweep
            AABB<T> linebox = line.getBBox();
            Vec2<T> aabbmax = aabb.pos + aabb.size;
            Vec2<T> lineMax = linebox.pos + linebox.size;

            // X axis overlap
            if (vel.x < 0) //Sweeping left
            {
                if (aabbmax.x < linebox.pos.x)
                    return Intersection<T>();
                times[0] = std::max((lineMax.x - aabb.pos.x) / vel.x, times[0]);
                times[1] = std::min((linebox.pos.x - aabbmax.x) / vel.x, times[1]);
            }
            else if (vel.x > 0) //Sweeping right
            {
                if (aabb.pos.x > lineMax.x)
                    return Intersection<T>();
                times[0] = std::max((linebox.pos.x - aabbmax.x) / vel.x, times[0]);
                times[1] = std::min((lineMax.x - aabb.pos.x) / vel.x, times[1]);
            }
            else
                if (linebox.pos.x > aabbmax.x || lineMax.x < aabb.pos.x)
                    return Intersection<T>();

            if (times[0] > times[1])
                return Intersection<T>();

            // Y axis overlap
            if (vel.y < 0) //Sweeping down
            {
                if (aabbmax.y < linebox.pos.y)
                    return Intersection<T>();
                times[0] = std::max((lineMax.y - aabb.pos.y) / vel.y, times[0]);
                times[1] = std::min((linebox.pos.y - aabbmax.y) / vel.y, times[1]);
            }
            else if (vel.y > 0) //Sweeping up
            {
                if (aabb.pos.y > lineMax.y)
                    return Intersection<T>();
                times[0] = std::max((linebox.pos.y - aabbmax.y) / vel.y, times[0]);
                times[1] = std::min((lineMax.y - aabb.pos.y) / vel.y, times[1]);
            }
            else
                if (linebox.pos.y > aabbmax.y || lineMax.y < aabb.pos.y)
                    return Intersection<T>();

            if (times[0] < 0 || times[1] > 1)
                return Intersection<T>();
        }

        if (times[0] > times[1])
            return Intersection<T>();

        // NOTE: if changing something related to normal directions, remember to change it in Line vs Line
        if (ndir == NormalLeft)
            ln = nd.left();
        else if (ndir == NormalRight)
            ln = nd.right();
        else
            ln = -ln;

        Intersection<T> isec((aabb.pos + vel * times[0]).asPoint(), times, ln);
        isec.type = SweptAABBxLine;
        return isec;
    }
}

#endif
