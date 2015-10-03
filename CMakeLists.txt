set(CXX_STANDARD_REQUIRED ON)

include_directories(include)
add_library(cppmath src/math/math.cpp)
set_property(TARGET cppmath PROPERTY CXX_STANDARD 11)
