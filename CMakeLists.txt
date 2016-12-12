cmake_minimum_required(VERSION 2.6)
project(cppmath)

set(CXX_STANDARD_REQUIRED ON)

option(BUILD_GRAPHICAL_TEST "Compile the graphical SFML testing program" OFF)
option(BUILD_TESTS "Build tests" OFF)

include_directories(include)
add_library(${PROJECT_NAME} src/math/math.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -Wno-switch)
elseif (MSVC)
    # Untested
    add_definitions(/W3)
endif()


if(BUILD_TESTS)
    enable_testing()
endif()

if(BUILD_TESTS OR BUILD_GRAPHICAL_TEST)
    add_subdirectory(test)
endif()
