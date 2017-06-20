cmake_minimum_required(VERSION 3.7)
project(NumericCatamorph)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

include_directories(boost_dependencies)
add_subdirectory(boost_dependencies/libs)

set(SOURCE_FILES main.cpp expression.h factories.h accessors.h
        functor_instance.h catamorphism.h printer.h eval.h
        dependencies.h optimizations.h partial_eval.h iverson.h)

enable_testing()
add_subdirectory(test)
add_executable(NumericCatamorph ${SOURCE_FILES})
target_link_libraries(NumericCatamorph MiniBoost)