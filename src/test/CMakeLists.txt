cmake_minimum_required(VERSION 3.2)
project(NumericCatamorph_tests)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)

set(TEST_SOURCES
        test_main.cpp
        polynomial_class_tests.cpp
        monoid_tests.cpp
        evmdd/numeric/evaluation_tests_arithmetic.cpp
        evmdd/numeric/construction_tests_arithmetic.cpp
        evmdd/numeric/evaluation_tests_iversion.cpp
        evmdd/numeric/construction_tests_iversion.cpp
        evmdd/conditional_effect_tests.cpp 
        evmdd/product_tests.cpp 
        evmdd/abstract_factory_test.cpp
	parser/base/infix_parser_tests.cpp
	parser/base/prefix_parser_tests.cpp
	parser/base/infix_to_prefix_tests.cpp
	parser/base/lexer_tests.cpp
	parser/complex/effect_parser_test.cpp
        )

add_executable(Test ${TEST_SOURCES})
target_link_libraries(Test NumericCatamorph_lib Catch)
add_test(NAME MyTest COMMAND Test)
