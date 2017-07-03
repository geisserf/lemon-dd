#!/bin/sh

set -e
./build.py -j4

./builds/debug32/bin/Benchmark
#pprof --callgrind builds/debug32/bin/Benchmark MyProfileFilename >profile.callgrind
#kcachegrind profile.callgrind

#pprof builds/debug32/bin/Benchmark /tmp/prof.out
#pprof --gv  builds/debug32/bin/Benchmark /tmp/prof.out