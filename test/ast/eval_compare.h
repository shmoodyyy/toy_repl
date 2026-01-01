#ifndef TEST_AST_EVAL_H
#define TEST_AST_EVAL_H

#include "test.h"
#include "frontend/ast.h"

TEST_CASE(test_eval_compare_eq_int())
{
    struct ASTNode lhs = {
        .type = ASTNODE_VALUE,
        .underlying.value = {
            .type = VALUE_INTEGER,
            .value.integer = 4
        }
    };
    struct ASTNode rhs = {
        .type = ASTNODE_VALUE,
        .underlying.value = {
            .type = VALUE_INTEGER,
            .value.integer = 4
        }
    };
    struct ASTNode compare = {
        .type = ASTNODE_COMPARE,
        .underlying.compare_op = {
            .type = COMP_OP_EQ,
            .left = &lhs,
            .right = &rhs,
        },
    };
    struct ASTNode result;

    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    1);

    lhs.underlying.value.value.integer = 6;
    rhs.underlying.value.value.integer = 6;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    1);

    lhs.underlying.value.value.integer = 1;
    rhs.underlying.value.value.integer = 6;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    0);

    lhs.underlying.value.value.integer = 1;
    rhs.underlying.value.value.integer = 2;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    0);

    lhs.underlying.value.value.integer = 1;
    rhs.underlying.value.value.integer = 1;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    1);
}

TEST_CASE(test_eval_compare_ne_int())
{
    struct ASTNode lhs = {
        .type = ASTNODE_VALUE,
        .underlying.value = {
            .type = VALUE_INTEGER,
            .value.integer = 50
        }
    };
    struct ASTNode rhs = {
        .type = ASTNODE_VALUE,
        .underlying.value = {
            .type = VALUE_INTEGER,
            .value.integer = 1
        }
    };
    struct ASTNode compare = {
        .type = ASTNODE_COMPARE,
        .underlying.compare_op = {
            .type = COMP_OP_NE,
            .left = &lhs,
            .right = &rhs,
        },
    };
    struct ASTNode result;

    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    1);

    lhs.underlying.value.value.integer = 5;
    rhs.underlying.value.value.integer = 6;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    1);

    lhs.underlying.value.value.integer = 6;
    rhs.underlying.value.value.integer = 6;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    0);

    lhs.underlying.value.value.integer = 2;
    rhs.underlying.value.value.integer = 2;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    0);

    lhs.underlying.value.value.integer = 100;
    rhs.underlying.value.value.integer = 1;
    result = eval(&compare, 0);
    ASSERT_EQ(result.type,                              ASTNODE_VALUE);
    ASSERT_EQ(result.underlying.value.type,             VALUE_BOOLEAN);
    ASSERT_EQ(result.underlying.value.value.boolean,    1);
}

#endif
