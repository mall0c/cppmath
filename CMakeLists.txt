cmake_minimum_required(VERSION 2.6)
project(cppmath)

set(CXX_STANDARD_REQUIRED OFF)

include_directories(include)
add_library(${PROJECT_NAME} src/math/math.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
