#!/usr/bin/env sh

mkdir -p out
SOURCES=$(find src -type f -regex ".*\.[c]" | tr '\n' ' ')
gcc -o ./out/parser -g -Wall -Isrc ${SOURCES}
