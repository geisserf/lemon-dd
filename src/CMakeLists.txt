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
#set(CMAKE_CXX_FLAGS_DEBUG "-O3")
#set(CMAKE_CXX_FLAGS_PROFILE "-O3 -pg")

include_directories(boost_dependencies)
add_subdirectory(boost_dependencies/libs)


set(SOURCE_FILES
        catamorph/Catamorph.cpp catamorph/Catamorph.h
        catamorph/Factories.cpp catamorph/Factories.h
        catamorph/interpreters/Optimizations.cpp catamorph/interpreters/Optimizations.h
        catamorph/interpreters/Evaluate.cpp catamorph/interpreters/Evaluate.h
        catamorph/interpreters/Dependencies.cpp catamorph/interpreters/Dependencies.h
        catamorph/interpreters/Iverson.cpp catamorph/interpreters/Iverson.h
        catamorph/Expression.h
        Polynomial.cpp Polynomial.h
        parser.cpp parser.h
        catamorph/Printer.h InfixParser.cpp InfixParser.h)

add_library(NumericCatamorph_lib ${SOURCE_FILES})
target_link_libraries(NumericCatamorph_lib MiniBoost)

add_subdirectory(evmdd)
add_subdirectory(test)
add_subdirectory(benchmark)
