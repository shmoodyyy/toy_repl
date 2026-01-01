#!/usr/bin/env sh

set -e

rm -f test/main.c
bash test/genmain.sh

mkdir -p out
SOURCES=$(find "src" -type f -regex ".*\.[c]" | grep -v "main.c" | tr '\n' ' ')
SOURCES_TEST=$(find "test" -type f -regex ".*\.[c]" | tr '\n' ' ')
gcc -o ./test/runtest -g -Wall -Isrc -Itest ${SOURCES} ${SOURCES_TEST}
test/runtest
