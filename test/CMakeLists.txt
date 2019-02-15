set(CXX_STANDARD_REQUIRED ON)

macro(gen_test TESTNAME SOURCE)
    add_executable(${TESTNAME} ${SOURCE})
    # target_link_libraries(${TESTNAME} ${PROJECT_NAME})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_property(TARGET ${TESTNAME} PROPERTY CXX_STANDARD 11)
endmacro()

if(CPPMATH_BUILD_GRAPHICAL_TEST)
    add_executable(graphical_test graphical.cpp)
    target_link_libraries(graphical_test sfml-graphics sfml-window sfml-system)
    set_property(TARGET graphical_test PROPERTY CXX_STANDARD 11)

    add_executable(sweep_graphical_test sweepgraphical.cpp)
    target_link_libraries(sweep_graphical_test sfml-graphics sfml-window sfml-system)
    set_property(TARGET sweep_graphical_test PROPERTY CXX_STANDARD 11)
endif()

if(CPPMATH_BUILD_TESTS)
    gen_test(parallel parallel.cpp)
    gen_test(vector_optypes vector_optypes.cpp)
    gen_test(polygonadapter polygonadapter.cpp)
endif()
