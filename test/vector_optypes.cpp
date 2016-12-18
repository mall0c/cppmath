#include "math/geometry/Vector.hpp"

using namespace math;

int main(int argc, char *argv[])
{
    Vec2f a;
    Vec2i b;
    Vec2<double> c;
    float u;

    static_assert(std::is_same<decltype((a+b)[0]), float&>::value,
            "Type should be float");

    static_assert(std::is_same<decltype((a-b)[0]), float&>::value,
            "Type should be float");

    static_assert(std::is_same<decltype((c/b)[0]), double&>::value,
            "Type should be double");

    static_assert(std::is_same<decltype((c*u)[0]), double&>::value,
            "Type should be double");

    static_assert(std::is_same<decltype((b*u)[0]), float&>::value,
            "Type should be float");

    static_assert(std::is_same<decltype((b*b)[0]), int&>::value,
            "Type should be int");

    return 0;
}
