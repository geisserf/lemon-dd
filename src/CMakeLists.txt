cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)

# Respect the PATH environment variable when searching for compilers.
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)


project(NumericCatamorph_lib)


set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Profile"
        CACHE STRING "Reset the configurations to what we need" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")

set(CMAKE_CXX_FLAGS_PROFILE ${CMAKE_CXX_FLAGS_DEBUG})
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")
set(CMAKE_CXX_FLAGS_PROFILE "-O3 -pg")

include_directories(boost_dependencies)
add_subdirectory(boost_dependencies/libs)


set(SOURCE_FILES catamorph/catamorph.cpp
        catamorph/catamorph.h
        catamorph/factories.cpp
        catamorph/factories.h
        catamorph/interpreters/optimizations.cpp
        catamorph/interpreters/optimizations.h
        catamorph/interpreters/evaluate.cpp
        catamorph/interpreters/evaluate.h
        catamorph/interpreters/dependencies.cpp
        catamorph/interpreters/dependencies.h
        catamorph/interpreters/iverson.cpp
        catamorph/interpreters/iverson.h catamorph/expression.h)

add_library(NumericCatamorph_lib ${SOURCE_FILES})
target_link_libraries(NumericCatamorph_lib MiniBoost)

add_subdirectory(test)