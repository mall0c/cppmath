cmake_minimum_required(VERSION 2.6)
project(cppmath)

set(CXX_STANDARD_REQUIRED ON)

option(CPPMATH_BUILD_GRAPHICAL_TEST "Compile the graphical SFML testing program" OFF)
option(CPPMATH_BUILD_TESTS "Build tests" OFF)

include_directories(include)
add_library(${PROJECT_NAME} src/math/math.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -Wno-switch -Wno-parentheses -Wsuggest-override -Werror=suggest-override)
elseif (MSVC)
    # Untested
    add_definitions(/W3)
endif()


if(CPPMATH_BUILD_TESTS)
    enable_testing()
endif()

if(CPPMATH_BUILD_TESTS OR CPPMATH_BUILD_GRAPHICAL_TEST)
    add_subdirectory(test)
endif()
