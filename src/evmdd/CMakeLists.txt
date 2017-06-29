cmake_minimum_required(VERSION 3.7)
project(evmdd_lib)


set(EVMDD_SOURCES
        Evmdd.h Node.h)


add_library(evmdd_lib ${EVMDD_SOURCES})
target_link_libraries(evmdd_lib NumericCatamorph_lib)