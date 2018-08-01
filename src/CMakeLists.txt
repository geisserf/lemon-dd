cmake_minimum_required(VERSION 3.5)
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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -Wfatal-errors")

set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "-O1")
set(CMAKE_CXX_FLAGS_PROFILE "-O3 -pg")

include_directories(boost_dependencies)
add_subdirectory(boost_dependencies/libs)


set(SOURCE_FILES
     catamorph/catamorph.h
     catamorph/factories.cpp catamorph/factories.h
     catamorph/interpreters/optimizations.cpp catamorph/interpreters/optimizations.h
     catamorph/interpreters/evaluate.cpp catamorph/interpreters/evaluate.h
     catamorph/interpreters/dependencies.cpp catamorph/interpreters/dependencies.h
     catamorph/interpreters/iverson.cpp catamorph/interpreters/iverson.h
     catamorph/interpreters/create_evmdd.h
     catamorph/expression.h
     globals.h
     polynomial.cpp polynomial.h
     logical_formula.cpp logical_formula.h
     parser.cpp parser.h
     effect_parser.h effect_parser.cpp
     conditional_effects.h conditional_effects.cpp
     catamorph/printer.h
     evmdd/evmdd.h
     evmdd/evmdd_cache.h
     evmdd/abstract_factory.h
     evmdd/product_evmdd.h evmdd/product_evmdd.cpp
     evmdd/node.h evmdd/node_factory.h
     evmdd/printer.h
     evmdd/monoid.h
     evmdd/monoids/product.h
     evmdd/monoids/numbers.cpp
     evmdd/monoids/facts.h evmdd/monoids/facts.cpp
     evmdd/monoids/boolean.h evmdd/monoids/boolean.cpp
     evmdd/operations/logic_and.h
     evmdd/operations/logic_or.h
     evmdd/operations/logic_not.h
     evmdd/operations/plus.h
     evmdd/operations/minus.h
     evmdd/operations/multiplies.h
     evmdd/operations/divides.h
     evmdd/operations/equal_to.h
     #utils/math_utils.h utils/math_utils.cpp
     #utils/system_utils.h utils/system_utils.cpp
     utils/string_utils.h
    )

add_library(NumericCatamorph_lib ${SOURCE_FILES})
target_link_libraries(NumericCatamorph_lib MiniBoost)

add_executable(Parser parser_main.cpp ${SOURCE_FILES})
add_executable(NumericCatamorph main.cpp ${SOURCE_FILES})
target_link_libraries(NumericCatamorph NumericCatamorph_lib)

add_subdirectory(test)
