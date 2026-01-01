#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int __last_test_result = 0;

#define ASSERT_EQ(a, b)\
    if (!(a == b)) {\
        __last_test_result = -1;\
        printf(RED "\n  assertion failed\n    " #a " == " #b);\
        printf(RED "\n    expected: %i", b);\
        printf(RED "\n    actual: %i", a);\
    }

#define ASSERT_NE(a, b)\
    if (!(a != b)) {\
        __last_test_result = -1;\
        printf(RED "\n  assertion failed\n    " #a " != " #b);\
        printf(RED "\n    expected: %i", b);\
        printf(RED "\n    actual: %i", a);\
    }

#define TEST_CASE(x) void x;\
void run_test_##x {\
    printf(RESET "running test '" YEL #x RESET "'...");\
    __last_test_result = 0; \
    x; \
    if (__last_test_result != 0)\
        printf(YEL "\n" #x RED " failed\n" RESET);\
    else\
        printf(GRN "ok.\n" RESET);\
}\
void x 

#endif
