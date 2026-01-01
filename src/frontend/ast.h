#ifndef FRONTEND_AST_NODE_H
#define FRONTEND_AST_NODE_H

#include <stdlib.h>

enum ValueType {
    VALUE_BOOLEAN   = 0x1,
    VALUE_INTEGER   = 0x2,
    VALUE_STRING    = 0x4,
    //VALUE_POINTER,
    //VALUE_REFERENCE,
};
struct Value
{
    enum ValueType type;
    union {
        int boolean;
        int integer;
        const char* string;

        size_t pointer_address;
        const char* reference_name;
    } value;
};

enum ASTNode_Type {
    ASTNODE_NONE = 0,

    ASTNODE_VALUE,
    ASTNODE_VARIABLE,

    ASTNODE_BINARY_OP,
    ASTNODE_BLOCK,

    ASTNODE_COMPARE,
    ASTNODE_IF,

    ASTNODE_LET,
};
enum BinaryOpType {
    BIN_OP_NONE = 0,

    BIN_OP_ADD,
    BIN_OP_SUB,
    BIN_OP_MUL,
    BIN_OP_DIV,
    BIN_OP_MOD,
};
struct ASTBinaryOp {
    enum BinaryOpType type;
    struct ASTNode* left;
    struct ASTNode* right;
};
enum CompOpType {
    COMP_OP_NONE = 0,

    COMP_OP_EQ,
    COMP_OP_NE,
    COMP_OP_LT,
    COMP_OP_LE,
    COMP_OP_GT,
    COMP_OP_GE,
};
struct ASTCompareOp {
    enum CompOpType type;
    struct ASTNode* left;
    struct ASTNode* right;
};
struct ASTNode {
    enum ASTNode_Type type;
    union {
        struct Value value;
        struct ASTBinaryOp binary_op;
        struct ASTCompareOp compare_op;
    } underlying;
};

enum ASTNode_ErrorType {
    AST_ERROR_NONE,
    AST_ERROR_NODE_MISMATCH,

    AST_ERROR_CMP_TYPE_MISMATCH,
};

struct ASTNode_Error {
    enum ASTNode_ErrorType type;
    union {
        struct {
            enum ASTNode_Type expected;
            enum ASTNode_Type actual;
        } ast_node_mismatch;
    } info;
};

struct ASTNode_Error_Array {
    struct ASTNode_Error* heap;
    int capacity;
    int size;
};

struct ASTNode_Error_Array create_error_array(int capacity);
void error_array_resize(struct ASTNode_Error_Array*, int capacity);
struct ASTNode_Error* error_array_push(struct ASTNode_Error_Array*);
struct ASTNode_Error* error_array_pop(struct ASTNode_Error_Array*);

struct ASTNode eval_value(struct Value*, struct ASTNode_Error_Array*);
struct ASTNode eval_compare(struct ASTCompareOp*, struct ASTNode_Error_Array*);
struct ASTNode eval_binary_op(struct ASTBinaryOp*, struct ASTNode_Error_Array*);
struct ASTNode eval(struct ASTNode*, struct ASTNode_Error_Array*);

#endif
