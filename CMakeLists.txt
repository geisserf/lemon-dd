cmake_minimum_required(VERSION 3.7)
project(Catamorph)

set(CMAKE_CXX_STANDARD 14)

include_directories( boost_dependencies )
add_subdirectory( boost_dependencies/libs )


set(SOURCE_FILES main.cpp
        expression.h
        factories.h accessors.h functor_instance.h catamorphism.h printer.h eval.h dependencies.h optimizations.h partial_eval.h iverson.h)

add_executable(Catamorph ${SOURCE_FILES})

target_link_libraries( ${SOME_EXECUTABLE} MiniBoost )