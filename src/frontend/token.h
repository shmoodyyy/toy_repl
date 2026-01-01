#ifndef FRONTEND_TOKEN_H
#define FRONTEND_TOKEN_H

typedef enum
{
    TOKEN_NONE = 0,
    TOKEN_PRINT,
    TOKEN_DECLARATION,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_EQUALS,
    TOKEN_NUMBER,

    TOKEN_OPERATOR,

    // exclusive value for next expected token
    TOKEN_ANY,
} TokenType;

const char* token_type_str(TokenType type)
{
    switch (type) {
        case TOKEN_NONE:        return "TOKEN_NONE"; 
        case TOKEN_PRINT:       return "TOKEN_PRINT"; 
        case TOKEN_OPERATOR:    return "TOKEN_OPERATOR"; 
        case TOKEN_DECLARATION: return "TOKEN_DECLARATION"; 
        case TOKEN_IDENTIFIER:  return "TOKEN_IDENTIFIER"; 
        case TOKEN_STRING:      return "TOKEN_STRING"; 
        case TOKEN_EQUALS:      return "TOKEN_EQUALS"; 
        case TOKEN_NUMBER:      return "TOKEN_NUMBER"; 
        case TOKEN_ANY:         return "TOKEN_ANY"; 
    }
}

typedef struct
{
    void* payload;
    TokenType type;
} Token;

#endif
