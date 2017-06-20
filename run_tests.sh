#!/bin/sh

set -e
./build.py --debug -j4

if [ -z "$*"]; then
    ./builds/debug32/bin/Test
fi
for TEST in "$@"
do
    ./builds/debug32/bin/Test $TEST
done
