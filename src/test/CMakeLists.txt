cmake_minimum_required(VERSION 3.5)
project(NumericCatamorph_tests)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)

set(TEST_SOURCES
        test_main.cpp
        simple_numeric_tests.cpp
        simple_iverson_tests.cpp
        polynomial_class_tests.cpp
        parser_tests.cpp
        monoid_tests.cpp
        evmdd_build_tests.cpp
       iverson_evmdd_tests.cpp conditional_effect_evmdd.cpp 
       combined_evmdd_tests.cpp infix_parser_tests.cpp effect_parser_test.cpp)

add_executable(Test ${TEST_SOURCES})
target_link_libraries(Test NumericCatamorph_lib Catch)
add_test(NAME MyTest COMMAND Test)
