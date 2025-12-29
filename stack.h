typedef enum
{
    TYPE_POINTER,
    TYPE_NUMBER
} Type;

typedef struct 
{
    const char* name;
    Type        type;
    union 
    {
        int     integral;
    } value;
} Variable;

typedef struct
{
    Variable* memory;
    int capacity;
    int size;
    int position;
} VariableStack;

VariableStack   create_stack(int capacity);
void            resize_stack(VariableStack* stack, int new_capacity);
Variable*       push_variable(VariableStack* stack, Variable variable);
void            pop_variable(VariableStack* stack);
Variable*       find_variable(VariableStack* stack, const char* name);
