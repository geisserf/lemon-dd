cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)

# Respect the PATH environment variable when searching for compilers.
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)


project(NumericCatamorph)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

include_directories(boost_dependencies)
add_subdirectory(boost_dependencies/libs)
add_subdirectory(test)


set(SOURCE_FILES main.cpp expression.h factories.h accessors.h
        functor_instance.h catamorphism.h printer.h eval.h
        dependencies.h optimizations.h partial_eval.h iverson.h)

add_executable(NumericCatamorph ${SOURCE_FILES})
target_link_libraries(NumericCatamorph MiniBoost)