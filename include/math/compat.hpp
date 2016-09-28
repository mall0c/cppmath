#ifndef CPPMATH_CPP11_COMPAT_HPP
#define CPPMATH_CPP11_COMPAT_HPP

// Disable constexpr if not compiled with c++11 or incompatible Microsoft compiler is used.
#if (defined(__GNUC__) && __cplusplus < 201103) || (defined(_WIN32) && _MSC_VER<1900)
#define constexpr
#endif

#endif
