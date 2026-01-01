#ifndef TEST_OF_TEST_H
#define TEST_OF_TEST_H
#include "test.h"

TEST_CASE(test_assert_eq())
{
    ASSERT_EQ(1, 1);
    ASSERT_EQ(1.f, 1.f);
    {
        int x = 5;
        int y = 5;
        ASSERT_EQ(x, y);
    }
    {
        float x = 5.f;
        float y = 5.f;
        ASSERT_EQ(x, y);
    }
}

TEST_CASE(test_assert_ne())
{
    ASSERT_NE(1, 2);
    ASSERT_NE(1.f, 20.f);
    {
        int x = 5;
        int y = 1;
        ASSERT_NE(x, y);
    }
    {
        float x = 5.f;
        float y = 25.f;
        ASSERT_NE(x, y);
    }
}

#endif
