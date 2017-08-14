cmake_minimum_required(VERSION 3.5) 
project(NumericCatamorph_benchmark)

set(BENCHMARK_SOURCES benchmark.cpp)
set(EVMDD_BENCHMARK_SOURCES evmdd_create_benchmark.cpp)

add_executable(Benchmark ${BENCHMARK_SOURCES})
add_executable(EvmddBenchmark ${EVMDD_BENCHMARK_SOURCES})
target_link_libraries(Benchmark NumericCatamorph_lib profiler)
target_link_libraries(EvmddBenchmark NumericCatamorph_lib profiler stdc++fs)
