#!/usr/bin/env sh
cd -- "$(dirname "$0")" >/dev/null 2>&1

set -e
rm -f main.c
touch main.c
TESTS=$(grep -R "TEST_CASE" | grep -v "TEST_CASE(x)" | sed "s/.*TEST_CASE(\(.*\))/\1/")
INCLUDES=$(grep -R "TEST_CASE" | grep -v "TEST_CASE(x)" | sed "s/\(.*\.h\):.*/\1/")

echo "#include \"test.h\"" > main.c
for i in ${INCLUDES}; do
    echo "#include \"${i}\"" >> main.c
done
echo "" >> main.c
echo "int main(int argc, const char** argv)" >> main.c
echo "{" >> main.c
for t in ${TESTS}; do
    echo "    run_test_${t};" >> main.c
done
echo "    return 0;" >> main.c
echo "}" >> main.c
