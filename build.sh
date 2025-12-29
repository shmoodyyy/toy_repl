mkdir out
SOURCES=$(find . -type f -regex ".*\.[c]" | tr '\n' ' ')
gcc -o ./out/parser -g -Wall ${SOURCES}
