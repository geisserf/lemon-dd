cmake_minimum_required(VERSION 3.7)
project(NumericCatamorph_benchmark)


set(BENCHMARK_SOURCES
        benchmark.cpp)


add_executable(Benchmark ${BENCHMARK_SOURCES})
target_link_libraries(Benchmark NumericCatamorph_lib profiler)