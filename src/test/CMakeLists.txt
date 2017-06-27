cmake_minimum_required(VERSION 3.7)
project(NumericCatamorph_tests)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)

set(TEST_SOURCES
        test_main.cpp
        simple_numeric_tests.cpp
        simple_iverson_tests.cpp
        polynomial_class_tests.cpp 
        parser_tests.cpp)

add_executable(Test ${TEST_SOURCES})
target_link_libraries(Test NumericCatamorph_lib Catch)
add_test(NAME MyTest COMMAND Test)
