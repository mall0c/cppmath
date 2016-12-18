#include "math/geometry/Line2.hpp"
#include "math/math.hpp"
#include <cassert>
#include <cstdlib>
#include <time.h>
#include <iostream>

using namespace math;
using namespace std;

template <class T>
T getRandom();

template <class T>
void printline(const char* name, const Line2<T>& line);

template <class T>
void error(const char* str, const Line2<T>& a, const Line2<T>& b);

template <class T>
bool testIntersect(LineType typea, LineType typeb);

template <class T>
bool testCombos();

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (!testCombos<double>())
        return 1;

    if (!testCombos<float>())
        return 1;

    if (!testCombos<int>())
        return 1;

    return 0;
}

template <class T>
void error(const char* str, const Line2<T>& a, const Line2<T>& b)
{
    cerr<<"-----------------------------------\nError: "<<str<<endl;
    printline("a", a);
    printline("b", b);
    cerr<<"a.d x b.d: "<<a.d.cross(b.d)<<endl;
    cerr<<"tolerance: "<<epsilon<T>()<<endl;
    cerr<<"equals zero: "<<almostEquals(a.d.cross(b.d), (T)0)<<endl;
}

template <class T>
void printline(const char* name, const Line2<T>& line)
{
    cerr<<name<<(line.type == Ray ? "(ray)" : line.type == Line ? "(line)" : "(segment)");
    cerr<<": ("<<line.p.x<<"; "<<line.p.y<<") + u*("<<line.d.x<<"; "<<line.d.y<<")\n";
}

template <class T>
bool testIntersect(LineType typea, LineType typeb)
{
    Line2<T> a(typea);
    Line2<T> b(typeb);

    for (int j = 0; j < 10; ++j)
    {
        a.p.set(getRandom<T>(), getRandom<T>());
        a.d.fill(getRandom<T>(), getRandom<T>());

        for (int i = 0; i < 100; ++i)
        {
            // randomize b's position and direction vector but keep it on a
            Vec2<T> u = a.d * getRandom<T>();
            b.p = a.p + u;
            b.d = a.d * getRandom<T>();

            if (a.intersect(b))
            {
                error("Intersection detected.", a, b);
                return false;
            }
            else if (typea == Ray && typeb == Ray &&
                    u.signs() != a.d.signs() && -u.signs() != b.d.signs())
            {
                if (!a.isParallel(b))
                {
                    error("Rays should be parallel but aren't.", a, b);
                    return false;
                }
            }
            else if (!a.isParallel(b))
            {
                error("Lines should be parallel but aren't.", a, b);
                return false;
            }
        }
    }
    return true;
}

template <class T>
bool testCombos()
{
    // Previously it was necessary checking the different line types.
    // But since isIdentical() has been removed it doesn't matter, but maybe
    // it will come in handy some day.
    return testIntersect<T>(Line, Line) &&
        testIntersect<T>(Line, Ray) &&
        testIntersect<T>(Ray, Ray);
}

template <class T>
T getRandom()
{
    const T x = (1000 - rand() % 2000);
    return (!std::is_integral<T>::value && std::abs(x) > 100) ? x / (1 + (T)(rand() % 500)) : x;
}
