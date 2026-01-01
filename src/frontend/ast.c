#include "ast.h"
#include "sy_assert.h"

#include <string.h>

struct ASTNode_Error_Array create_error_array(int capacity)
{
    struct ASTNode_Error* heap = malloc(capacity * sizeof(*heap));
    return (struct ASTNode_Error_Array){
        .heap = heap,
        .capacity = capacity,
        .size = 0
    };
}

void error_array_resize(struct ASTNode_Error_Array* array, int capacity)
{
    if (array->capacity >= capacity) return;

    struct ASTNode_Error* new_heap = malloc(capacity * sizeof(*new_heap));
    if (!new_heap) abort();

    memcpy(new_heap, array->heap, array->size * sizeof(*array->heap));
    free(array->heap);
    array->heap = new_heap;

    return;
}

struct ASTNode_Error* error_array_push(struct ASTNode_Error_Array* array)
{
    if (array->size >= array->capacity)
        error_array_resize(array, array->capacity * 2);
    return &array->heap[array->size];
}

struct ASTNode_Error* error_array_pop(struct ASTNode_Error_Array* array)
{
    if (array->size <= 0)
        return 0;
    return &array->heap[--array->size];
}

struct ASTNode eval_value(struct Value* value, struct ASTNode_Error_Array* error_array)
{
    return (struct ASTNode){
        .type = ASTNODE_VALUE,
        .underlying.value = *value,
    };
}

struct ASTNode eval_compare(struct ASTCompareOp* compare, struct ASTNode_Error_Array* error_array)
{
    struct ASTNode eval_left = eval(compare->left, error_array);
    struct ASTNode eval_right = eval(compare->right, error_array);
    int good = 1;
    if (eval_left.type != ASTNODE_VARIABLE && eval_left.type != ASTNODE_VALUE) {
        good = 0;
        if (error_array > 0) {
            struct ASTNode_Error* err = error_array_push(error_array);
            err->type = AST_ERROR_NODE_MISMATCH;
            err->info.ast_node_mismatch.expected = ASTNODE_VALUE;
            err->info.ast_node_mismatch.actual = eval_left.type;
        }
    }
    if (eval_right.type != ASTNODE_VARIABLE && eval_right.type != ASTNODE_VALUE) {
        good = 0;
        if (error_array > 0) {
            struct ASTNode_Error* err = error_array_push(error_array);
            err->type = AST_ERROR_NODE_MISMATCH;
            err->info.ast_node_mismatch.expected = ASTNODE_VALUE;
            err->info.ast_node_mismatch.actual = eval_right.type;
        }
    }
    if (good <= 0) return (struct ASTNode){};


    struct Value* lhs = &eval_left.underlying.value;
    struct Value* rhs = &eval_right.underlying.value;
    if (lhs->type != rhs->type) {
        struct ASTNode_Error* err = error_array_push(error_array);
        err->type = AST_ERROR_CMP_TYPE_MISMATCH;
        err->info.ast_node_mismatch.expected = ASTNODE_VALUE;
        err->info.ast_node_mismatch.actual = eval_right.type;
        return (struct ASTNode){};
    }

    struct Value result = { .type = VALUE_BOOLEAN };
    switch (compare->type) {
    case COMP_OP_EQ:
        switch (lhs->type) {
        case VALUE_BOOLEAN: result.value.boolean = (lhs->value.boolean == rhs->value.boolean);          break;
        case VALUE_INTEGER: result.value.boolean = (lhs->value.integer == rhs->value.integer);          break;
        case VALUE_STRING:  result.value.boolean = strcmp(lhs->value.string, rhs->value.string) == 0;   break;
        } break;

    case COMP_OP_NE:
        switch (lhs->type) {
        case VALUE_BOOLEAN: result.value.boolean = (lhs->value.boolean != rhs->value.boolean);          break;
        case VALUE_INTEGER: result.value.boolean = (lhs->value.integer != rhs->value.integer);          break;
        case VALUE_STRING:  result.value.boolean = strcmp(lhs->value.string, rhs->value.string) != 0;   break;
        } break;

    case COMP_OP_LT:
        switch (lhs->type) {
        case VALUE_BOOLEAN: result.value.boolean = (lhs->value.boolean < rhs->value.boolean);           break;
        case VALUE_INTEGER: result.value.boolean = (lhs->value.integer < rhs->value.integer);           break;
        case VALUE_STRING:  result.value.boolean = strcmp(lhs->value.string, rhs->value.string) < 0;    break;
        } break;

    case COMP_OP_LE:
        switch (lhs->type) {
        case VALUE_BOOLEAN: result.value.boolean = (lhs->value.boolean <= rhs->value.boolean);          break;
        case VALUE_INTEGER: result.value.boolean = (lhs->value.integer <= rhs->value.integer);          break;
        case VALUE_STRING:  result.value.boolean = strcmp(lhs->value.string, rhs->value.string) <= 0;   break;
        } break;

    case COMP_OP_GT:
        switch (lhs->type) {
        case VALUE_BOOLEAN: result.value.boolean = (lhs->value.boolean > rhs->value.boolean);           break;
        case VALUE_INTEGER: result.value.boolean = (lhs->value.integer > rhs->value.integer);           break;
        case VALUE_STRING:  result.value.boolean = strcmp(lhs->value.string, rhs->value.string) > 0;    break;
        } break;

    case COMP_OP_GE:
        switch (lhs->type) {
        case VALUE_BOOLEAN: result.value.boolean = (lhs->value.boolean >= rhs->value.boolean);          break;
        case VALUE_INTEGER: result.value.boolean = (lhs->value.integer >= rhs->value.integer);          break;
        case VALUE_STRING:  result.value.boolean = strcmp(lhs->value.string, rhs->value.string) >= 0;   break;
        } break;

    default:
        abort_impl_bug("not implemented");

    case COMP_OP_NONE:
        abort_impl_bug("should not reach COMP_OP_NONE case");
    }
    return (struct ASTNode) {
        .type = ASTNODE_VALUE,
        .underlying.value = result,
    };
}

struct ASTNode eval_binary_op(struct ASTBinaryOp* node, struct ASTNode_Error_Array* error_array)
{
    return (struct ASTNode){};
}

struct ASTNode eval(struct ASTNode* node, struct ASTNode_Error_Array* error_array)
{
    switch (node->type) {
    case ASTNODE_COMPARE:       return eval_compare(&node->underlying.compare_op, error_array);
    case ASTNODE_BINARY_OP:     return eval_binary_op(&node->underlying.binary_op, error_array);
    case ASTNODE_VALUE:
        return (struct ASTNode){
            .type = ASTNODE_VALUE,
            .underlying.value = node->underlying.value,
        };
    }

    return (struct ASTNode){0};
}
