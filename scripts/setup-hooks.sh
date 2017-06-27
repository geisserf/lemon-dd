#!/bin/sh
root="`git rev-parse --show-toplevel`"
dir="$root/.git"

cp $root/scripts/pre-commit-clang-format $dir/hooks
echo "hook script set up in $dir/hooks" 
