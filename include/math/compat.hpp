#ifndef CPPMATH_CPP11_COMPAT_HPP
#define CPPMATH_CPP11_COMPAT_HPP

// Disable constexpr when compiling with an incompatible Microsoft compiler.
#if (defined(_WIN32) && _MSC_VER<1900)
#define constexpr
#endif

#endif
