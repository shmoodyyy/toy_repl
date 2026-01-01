#include "stack.h"
#include "frontend/ast.h"
#include "frontend/token.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define PREFIX "<sy> "

char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

Token try_parse(const char* in, int* pos, const char* token_str, TokenType expected)
{
    int i = 0;
    char c = to_lower(*(in + *pos));
    while (c != '\0' && c != ' ') {
        if (c != to_lower(*(token_str + i))) return (Token) {0}; 
        i += 1;
        c = *(in + *pos + i);
        if (*(token_str + i) == '\0') break;
    }
    *pos += i;
    return (Token) {
        .type = expected
    };
}

int is_numeric(char c)
{
    return (c >= '0' && c <= '9');
}

int char_valid_literal(char c, int is_first)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (is_first && (c >= '0' || c <= '9'));
}

Token try_parse_identifier(const char* in, int* pos)
{
    int i = 0;
    char c = *(in + *pos + i);
    if (c == '\n') return (Token) {0};
    if (is_numeric(c)) return (Token) {0};
    while (char_valid_literal(c, i == 0)) {
        i += 1;
        c = *(in + *pos + i);
    }
    if (i <= 0) return (Token) {0};
    char* payload = malloc(i + 1);
    memcpy(payload, in + *pos, i);
    for (int j = 0; j < i - 1; ++j)
        if (payload[j] == '\n')
            payload[j] = ' ';
    payload[i] = '\0';

    *pos += i;
    return (Token) {
        .payload    = payload,
        .type       = TOKEN_STRING
    };
}

Token try_parse_string(const char* in, int* pos)
{
    int i = 0;
    char escape_quote;
    char c = *(in + *pos + i);
    if (c == '\n') return (Token) {0};

    if (c == '"' || c == '\'') {
        escape_quote = c;
        *pos += 1;
    } else { return (Token) {0}; }

    int in_string = 1;
    while (in_string) {
        char previous = c;
        i += 1;
        c = *(in + *pos + i);
        if (c == escape_quote && previous != '\\') {
            i += 1;
            break;
        }
    }
    if (i <= 0) return (Token) {0};
    char* payload = malloc(i);
    memcpy(payload, in + *pos, i - 1);
    for (int j = 0; j < i - 1; ++j)
        if (payload[j] == '\n')
            payload[j] = ' ';
    payload[i] = '\0';

    *pos += i;
    return (Token) {
        .payload    = payload,
        .type       = TOKEN_STRING
    };
}

Token try_parse_numeric(const char* in, int* pos)
{
    int i = 0;
    char c = *(in + *pos + i);
    int has_sign = (c == '-' || c == '+');
    while ((i == 0 && has_sign) || is_numeric(c)) {
        if (!(i == 0 && has_sign) && !is_numeric(c)) return (Token) {0};
        i += 1;
        c = *(in + *pos + i);
    }
    if (i <= 0 || (has_sign && i <= 1)) return (Token) {0};
    int* payload = malloc(sizeof(int));
    *payload = atoi(in + *pos);
    *pos += i;
    return (Token) {
        .payload    = payload,
        .type       = TOKEN_NUMBER
    };
}

Token parse_next(const char* str, int* pos, int count)
{
    while (*(str + *pos) != '\0') {
        char c = to_lower(*(str + *pos));
        while (c == ' ') {
            (*pos) += 1;
            c = to_lower(*(str + *pos));
        }
        if (*pos >= count) return (Token) {0};

        Token parsed = { .type = TOKEN_NONE };
        do {
            if ((parsed = try_parse(str, pos, "print",  TOKEN_PRINT),       parsed.type != TOKEN_NONE)) break;
            if ((parsed = try_parse(str, pos, "=",      TOKEN_EQUALS),      parsed.type != TOKEN_NONE)) break;
            if ((parsed = try_parse(str, pos, "let",    TOKEN_DECLARATION), parsed.type != TOKEN_NONE)) break;
            if ((parsed = try_parse(str, pos, "+",      TOKEN_OPERATOR),    parsed.type != TOKEN_NONE)) break;
            if ((parsed = try_parse(str, pos, "-",      TOKEN_OPERATOR),    parsed.type != TOKEN_NONE)) break;

            if ((parsed = try_parse_string(str, pos),                       parsed.type != TOKEN_NONE)) break;
            if ((parsed = try_parse_identifier(str, pos),                   parsed.type != TOKEN_NONE)) break;
            if ((parsed = try_parse_numeric(str, pos),                      parsed.type != TOKEN_NONE)) break;
        } while (0);

        if (parsed.type != TOKEN_NONE) {
            return parsed;
        }

        (*pos) += 1;
    }
    return (Token) {0};
}

void eval_tokens(VariableStack* stack, Token* token_stack, int count)
{
    TokenType next_expected = TOKEN_ANY;
    if (0) {
        printf("token stack:\n");
        for (int i = 0; i < count; ++i) {
            if (i != 0) {
                printf(" -> ");
            }
            switch (token_stack[i].type) {
            case TOKEN_STRING:
                printf("%s { %s }", token_type_str(token_stack[i].type), (char*)token_stack[i].payload);
                break;

            case TOKEN_NUMBER:
                printf("%s { %i }", token_type_str(token_stack[i].type), *(int*)token_stack[i].payload);
                break;

            default:
                printf("%s", token_type_str(token_stack[i].type));
                break;
            }
        }
        printf("\n");
    }
    Token cur;
    for (int i = 0; i < count; ++i) {
        cur = token_stack[i];
        if (next_expected != TOKEN_ANY && cur.type != next_expected) {
            printf("eval error, expected token %s, got %s\n", token_type_str(next_expected), token_type_str(cur.type));
            if (cur.payload != 0)
                free(cur.payload);
            cur.payload = 0;
            return;
        }

        switch (cur.type) {
        case TOKEN_PRINT: {
            next_expected = TOKEN_STRING;
            i += 1; cur = token_stack[i];
            if (cur.type != next_expected) {
                printf("expected literal variable name print out\n");
                break;
            }
            char* name = cur.payload;
            Variable* var = find_variable(stack, name);
            if (var == 0)
                printf("variable '%s' undeclared\n", name);
            else
                printf("'%s' = %i\n", name, var->value.integral);

            if (cur.payload) {
                free(cur.payload);
                cur.payload = 0;
            }
            next_expected = TOKEN_ANY;
            break;
        }

        case TOKEN_DECLARATION: {
            next_expected = TOKEN_STRING;
            i += 1; cur = token_stack[i];
            if (cur.type != next_expected) {
                printf("expected literal variable name to follow 'let'\n");
                break;
            }
            next_expected = TOKEN_EQUALS;
            char* name = cur.payload;
            i += 1; cur = token_stack[i];
            if (cur.type != next_expected) {
                if (cur.payload != 0) {
                    printf("expected variable declaration for '%s' with '='\n", name);
                    free(cur.payload);
                    name = 0;
                }
                break;
            }
            next_expected = TOKEN_NUMBER;
            i += 1; cur = token_stack[i];
            if (cur.type != next_expected) {
                printf("expected numeric value for variable '%s'\n", name);
            }
            Variable* found = find_variable(stack, name);
            if (found != 0) {
                found->value.integral = *(int*)cur.payload;
            } else {
                push_variable(stack, (Variable){
                    .name = name,
                    .type = TYPE_NUMBER,
                    .value = { .integral = *(int*)cur.payload }
                });
            }
            if (cur.payload) {
                free(cur.payload);
                cur.payload = 0;
            }
            next_expected = TOKEN_ANY;
            break;
        }

        case TOKEN_IDENTIFIER:
            next_expected = TOKEN_EQUALS;
            break;

        case TOKEN_STRING:
            next_expected = TOKEN_EQUALS;
            break;

        case TOKEN_EQUALS:
            next_expected = TOKEN_NUMBER;
            break;

        case TOKEN_NUMBER:
            next_expected = TOKEN_ANY;
            break;

        case TOKEN_OPERATOR:
            next_expected = TOKEN_NUMBER;
            break;

        case TOKEN_ANY:
        case TOKEN_NONE:
            printf("unexpected token.\n");
            next_expected = TOKEN_ANY;
            break;
        }
    }
    if (next_expected != TOKEN_ANY && next_expected != TOKEN_NONE) {
        printf("abrupt expression end, expected %s\n", token_type_str(next_expected));
        if (cur.payload != 0) {
            free(cur.payload);
            cur.payload = 0;
        }
    }
}

void parse_and_eval(VariableStack* stack, const char* expression, int count)
{
    int offset = 0;
    int any_parsed = 0;

    Token token_stack[512];
    int token_pos = 0;
    while (offset < count) {
        while (*(expression + offset) == ' ') offset += 1;
        Token token = parse_next(expression, &offset, count);
        if (token.type != TOKEN_NONE) {
            token_stack[token_pos] = token;
            token_pos += 1;
            any_parsed = 1;
        }
    }

    if (any_parsed == 0) {
        printf("could not parse expression: %s", expression);
    }

    eval_tokens(stack, token_stack, token_pos);
}

int main(int argc, const char** argv)
{
    VariableStack stack = create_stack(1024);
    char buffer[BUF_SIZE];

    while (1) {
        memset(buffer, '\0', (size_t)BUF_SIZE);
        write(1, PREFIX, strlen(PREFIX));
        int bytes_read = read(0, buffer, BUF_SIZE);
        if (bytes_read <= 1) {
            printf("empty expression.\n");
            continue;
        }
        parse_and_eval(&stack, buffer, bytes_read);
    }
    return 0;
}
