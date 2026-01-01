#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

VariableStack create_stack(int capacity)
{
    Variable* memory = malloc(capacity * sizeof(Variable));
    return (VariableStack){
        .memory = memory,
        .capacity = capacity,
        .size = 0,
        .position = 0
    };
}

void resize_stack(VariableStack* stack, int new_capacity)
{
    if (stack->capacity >= new_capacity) return;

    Variable* new_stack = malloc(new_capacity * sizeof(Variable));
    if (stack->capacity > 0) {
        memcpy(new_stack, stack->memory, stack->size * sizeof(Variable));
    }
    free(stack->memory);
    stack->memory   = new_stack;
    stack->capacity = new_capacity;
}

Variable* push_variable(VariableStack* stack, Variable variable)
{
    if (stack->size >= stack->capacity)
        resize_stack(stack, stack->capacity * 2);
    Variable* new_variable = stack->memory + stack->size;
    memcpy(new_variable, &variable, sizeof(Variable));
    stack->size += 1;
    return new_variable;
}

void pop_variable(VariableStack* stack)
{
    Variable* end = stack->memory + stack->size - 1;
    memset(end, 0, sizeof(Variable));
    stack->size -= 1;
}

Variable* find_variable(VariableStack* stack, const char* name)
{
    int found;
    for (int i = stack->size - 1; i >= 0; --i) {
        found = 1;
        Variable* var = stack->memory + i;

        const char* name_find = name;
        const char* name_comp = var->name;
        while (*name_find != '\0' && *name_comp != '\0') {
            if (*name_find++ == *name_comp++) continue;

            found = 0;
            break;
        }
        if (found > 0) return var;
    }
    return 0;
}
