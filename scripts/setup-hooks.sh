#!/bin/sh
root="`git rev-parse --show-toplevel`"
dir="$root/.git"

cp $root/scripts/pre-commit-clang-format $dir/hooks/pre-commit
echo "pre-commit hook script set up in $dir/hooks" 
