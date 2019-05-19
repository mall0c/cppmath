#ifndef MATH_INTERSECT_FUNCTIONS_HPP
#define MATH_INTERSECT_FUNCTIONS_HPP

#include "Intersection.hpp"

namespace math
{
    template <typename T>
    class AbstractPolygon;

    template <typename T> bool            intersect(const Point2<T>& point, const Point2<T>& a, const Point2<T>& b, const Point2<T>& c);

    template <typename T> Intersection<T> intersect(const Line2<T>& line, const AbstractPolygon<T>& pol);
    template <typename T> bool            intersect(const Point2<T>& point, const AbstractPolygon<T>& pol);

    template <typename T> Intersection<T> intersect(const Line2<T>& line, const Line2<T>& other, NormalDirection ndir = NormalBoth);
    template <typename T> Intersection<T> intersect(const Line2<T>& line, const AABB<T>& box);
    template <typename T> bool            intersect(const Line2<T>& line, const Point2<T>& point);

    template <typename T> bool            intersect(const AABB<T>& aabb, const Point2<T>& point);
    template <typename T> Intersection<T> intersect(const AABB<T>& aabb, const AABB<T>& other);
    template <typename T> bool            contains(const AABB<T>& aabb, const AABB<T>& other);

    template <typename T> Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, AABB<T> other);
    template <typename T> Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, const AbstractPolygon<T>& pol, bool avgCorners = true, bool backfaceCulling = true);
    template <typename T> Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, const Line2<T>& line, NormalDirection ndir = NormalBoth);


    // Helpers

    // Returns nearest intersection
    // Basically the same as intersect(), but without early-out checks.
    // Is used internally by intersect().
    template <typename T>
    Intersection<T> findNearest(const Line2<T>& line, const AbstractPolygon<T>& pol);

    template <typename T>
    bool checkScale(const Line2<T>& line, double u);
}


#include "Polygon.hpp"
#include <cassert>

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
            auto isec = intersect(line, seg, pol.getNormalDir());
            if (!nearest || (isec && isec.time < nearest.time))
                nearest = isec;
            return false;
        };

        pol.foreachSegment(cb);
        return nearest;
    }

    template <class T>
    bool checkScale(const Line2<T>& line, double u)
    {
        if (line.type == Ray)
            return u >= 0;
        if (line.type == Segment)
            return u >= 0 && u <= 1;
        return true;
    }

    template <typename T>
    bool intersect(const Point2<T>& p, const Point2<T>& a, const Point2<T>& b, const Point2<T>& c)
    {
        auto pab = (p - a).cross(b - a);
        auto pbc = (p - b).cross(c - b);

        if (sign(pab) != sign(pbc))
            return false;

        return sign(pab) == sign((p - c).cross(a - c));
    }

    template <typename T>
    Intersection<T> intersect(const Line2<T>& line, const AbstractPolygon<T>& pol)
    {
        if (pol.size() < 2)
            return Intersection<T>();

        if (!intersect(line, pol.getBBox()))
            return Intersection<T>();

        auto isec = findNearest(line, pol);
        if (isec)
            return isec;

        // Segment fully inside
        if (pol.getFillType() == Filled && line.type == Segment && intersect(line.p, pol))
            return Intersection<T>(line.p, Vec2f(), Vec2f());

        return Intersection<T>();
    }


    template <typename T>
    bool intersect(const Point2<T>& point, const AbstractPolygon<T>& pol)
    {
        if (pol.size() < 2)
            return false;

        if (pol.size() == 2)
            return intersect(pol.getSegment(0, 1), point);

        if (!intersect(pol.getBBox(), point))
            return false;

        if (pol.getFillType() == Filled)
        {
            Line2<T> ray(point, Vec2<T>(1, 0), Ray);
            size_t num = 0;

            pol.foreachSegment([&](const Line2<T>& line) {
                if (intersect(ray, line))
                    ++num;
                return pol.isConvex() ? num == 2 : false;
            });

            return num % 2 == 1;
        }
        else
        {
            for (size_t i = 1; i < pol.size(); ++i)
                if (intersect(pol.getSegment(i - 1, i), point))
                    return true;
        }

        return false;
    }

    template <typename T>
    Intersection<T> sweep(const AABB<T>& aabb, const Vec2<T>& vel, AABB<T> other)
    {
        other.extend(aabb);
        auto isec = intersect(Line2<T>(aabb.getCenter(), vel, Segment), other);
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
            auto isec = sweep(aabb, vel, seg, pol.getNormalDir());
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

    template <typename T>
    Intersection<T> intersect(const Line2<T>& line, const Line2<T>& other, NormalDirection ndir)
    {
        if (line.isParallel(other))
            return Intersection<T>();

        const Vec2<T> dist = other.p - line.p;
        const double m = other.d.cross(line.d),
                     u = other.d.cross(dist) / m,
                     v = line.d.cross(dist) / m;

        if (!checkScale(line, u) || !checkScale(other, v))
            return Intersection<T>();


        // NOTE: if changing something related to normal directions, remember to change it in AABB vs Line sweep
        Vec2<T> normal = other.d.normalized();
        if (ndir == NormalLeft)
            normal = normal.left();
        else if (ndir == NormalRight)
            normal = normal.right();
        else
            normal = normal.left() * sign(m);
        return Intersection<T>(line.p + line.d * u, Vec2<T>(u, v), normal);
    }

    template <typename T>
    Intersection<T> intersect(const Line2<T>& line, const AABB<T>& box)
    {
        // Adapted from http://noonat.github.io/intersect/#aabb-vs-segment

        // If d contains a zero element, this stores the index of the
        // non-zero element. Otherwise -1.
        int j = -1;

        // Check if an element in d is 0.
        for (size_t i = 0; i < 2; ++i)
        {
            if (almostEquals(line.d[i], (T)0))
            {
                if (line.p[i] >= box.pos[i] && line.p[i] < box.pos[i] + box.size[i])
                {
                    j = (i + 1) % 2;
                    break;
                }
                return Intersection<T>();
            }
        }

        // near will be reused to store the final near and far time.
        Vec2d near;
        Vec2<T> signs = line.d.signs();
        Vec2<T> normal;

        if (j == -1)
        {
            Vec2d half = box.size / 2.0;
            near      = ((box.pos + half) - signs * half - line.p.asVector()) / line.d;
            Vec2d far = ((box.pos + half) + signs * half - line.p.asVector()) / line.d;

            if (near.x > far.y || near.y > far.x)
                return Intersection<T>();

            if (near.x > near.y)
                normal.x = -signs.x;
            else
                normal.y = -signs.y;

            near.x = std::max(near.x, near.y); // near time
            near.y = std::min(far.x, far.y); // far time
        }
        else
        {
            int s = signs[j];
            near.x = (box.pos[j] + ((s == 1) ? 0 : box.size[j]) - line.p[j]) / line.d[j];
            near.y = (box.pos[j] + ((s != 1) ? 0 : box.size[j]) - line.p[j]) / line.d[j];
            normal[j] = -s;
        }

        if (line.type != Line && near.y < 0)
            return Intersection<T>();

        if (line.type == Segment && near.x > 1)
            return Intersection<T>();

        if (line.type != Line && near.x < 0)
            near.x = 0;

        if (line.type == Segment && near.y > 1)
            near.y = 1;

        return Intersection<T>(line.p + line.d * near.x,
                               line.p + line.d * near.y,
                               near, normal);
    }

    template <typename T>
    bool intersect(const Line2<T>& line, const Point2<T>& point)
    {
        return almostZero((point - line.closestPoint(point, true)).abs_sqr());
    }


    template <typename T>
    bool intersect(const AABB<T>& aabb, const Point2<T>& point)
    {
        return point.asVector() >= aabb.pos && point.asVector() < aabb.pos + aabb.size;
    }

    template <typename T>
    Intersection<T> intersect(const AABB<T>& aabb, const AABB<T>& other)
    {
        // Adapted from http://noonat.github.io/intersect/#aabb-vs-aabb
        auto d = other.getCenter() - aabb.getCenter();
        auto p = (other.size / 2 + aabb.size / 2) - abs(d);

        if (p.x < 0 || p.y < 0)
            return Intersection<T>();

        return Intersection<T>(
                p * d.signs(),
                p.x < p.y ? Vec2<T>(-sign(d.x), 0) : Vec2<T>(0, -sign(d.y)));
    }

    template <typename T>
    bool contains(const AABB<T>& aabb, const AABB<T>& other)
    {
        return intersect(aabb, other.pos.asPoint())
            && intersect(aabb, other.pos.asPoint() + other.size);
    }
}

#endif
