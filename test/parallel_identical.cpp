#include "math/geometry/Line2.hpp"
#include "math/math.hpp"
#include <cassert>
#include <cstdlib>
#include <time.h>
#include <iostream>

using namespace geometry;
using namespace std;

template <class T>
T getRandom();

template <class T, bool ray>
void printline(const char* name, const Line2<T, ray>& line);

template <class T, bool rayA, bool rayB>
void error(const char* str, const Line2<T, rayA>& a, const Line2<T, rayB>& b);

template <class T, bool rayA, bool rayB>
bool testIntersect();

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (!testIntersect<double, false, false>())
        return 1;

    if (!testIntersect<double, false, true>())
        return 1;

    if (!testIntersect<double, true, true>())
        return 1;

    if (!testIntersect<float, false, false>())
        return 1;

    if (!testIntersect<float, false, true>())
        return 1;

    if (!testIntersect<float, true, true>())
        return 1;

    if (!testIntersect<int, false, false>())
        return 1;

    if (!testIntersect<int, false, true>())
        return 1;

    if (!testIntersect<int, true, true>())
        return 1;

    return 0;
}

template <class T, bool rayA, bool rayB>
void error(const char* str, const Line2<T, rayA>& a, const Line2<T, rayB>& b)
{
    cerr<<"-----------------------------------\nError: "<<str<<endl;
    printline("a", a);
    printline("b", b);
    cerr<<"a.d x b.d: "<<a.d.cross(b.d)<<endl;
    cerr<<"tolerance: "<<math::TypeTraits<T>::epsilon()<<endl;
    cerr<<"equals zero: "<<math::almostEquals(a.d.cross(b.d), (T)0)<<endl;
}

template <class T, bool ray>
void printline(const char* name, const Line2<T, ray>& line)
{
    cerr<<name<<(ray ? "(ray)" : "")<<": ("<<line.p.x<<"; "<<line.p.y<<") + u*("<<line.d.x<<"; "<<line.d.y<<")\n";
}

template <class T, bool rayA, bool rayB>
bool testIntersect()
{
    Line2<T, rayA> a;
    Line2<T, rayB> b;

    for (int j = 0; j < 10; ++j)
    {
        a.p.set(getRandom<T>(), getRandom<T>());
        a.d.fill(getRandom<T>(), getRandom<T>());

        for (int i = 0; i < 100; ++i)
        {
            // randomize b's position and direction vector but keep it on a
            Vector2<T> u = a.d * getRandom<T>();
            b.p = a.p + u;
            b.d = a.d * getRandom<T>();

            if (a.intersect(b))
            {
                error("Intersection detected.", a, b);
                return false;
            }
            else if (rayA && rayB && u.signs() != a.d.signs() && -u.signs() != b.d.signs())
            {
                if (a.isIdentical(b))
                {
                    error("Rays are identical but shouldn't be.", a, b);
                    return false;
                }
                else if (!a.isParallel(b))
                {
                    error("Rays should be parallel but aren't.", a, b);
                    return false;
                }
            }
            else if (a.isParallel(b))
            {
                error("Lines should not be parallel.", a, b);
                return false;
            }
            else if (!a.isIdentical(b))
            {
                error("Lines should be identical but aren't.", a, b);
                return false;
            }
        }
    }
    return true;
}

template <class T>
T getRandom()
{
    const T x = (1000 - rand() % 2000);
    return (!std::is_integral<T>::value && std::abs(x) > 100) ? x / (1 + (T)(rand() % 500)) : x;
}
