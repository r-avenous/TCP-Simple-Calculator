#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _stack_float
{
    int size;
    float *arr;
}stack_float;
typedef struct _stack_char
{
    char *arr;
    int size;
}stack_char;
void pushc(stack_char* st, char val)
{
    st->arr[st->size] = val;
    st->size++;
}
void pushf(stack_float* st, float val)
{
    st->arr[st->size] = val;
    st->size++;
}
char popc(stack_char *st)
{
    st->size--;
    return st->arr[st->size];
}
float popf(stack_float *st)
{
    st->size--;
    return st->arr[st->size];
}
float evaluateExpression(char* exp)
{
    char* string = (char*)malloc(3*strlen(exp));
    stack_char operators;
    operators.arr = (char*)malloc(strlen(exp));
    operators.size = 0;
    stack_float operands;
    operands.arr = (float*)malloc(strlen(exp)*sizeof(float));
    operands.size = 0;
    int index=0;
    for(int i=0; exp[i] != '\0'; i++)
    {
        if(exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '(' || exp[i] == ')')
        {
            string[index++] = ' ';
            string[index++] = exp[i];
            string[index++] = ' ';
        }
        else
            string[index++] = exp[i];
    }
    char* token = strtok(string, " ");
    while(token != NULL) 
    {
        // printf("%s\n", token);
        // printf("%d %d\n", operands.size, operators.size);
        if(token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')
        {
            pushc(&operators, token[0]);
        }
        else if(token[0] == '(')
        {
            pushc(&operators, token[0]);
        }
        else if(token[0] == ')')
        {
            popc(&operators);
            if(operators.size > 0)
            {
                float op2 = popf(&operands);
                float op1 = popf(&operands);
                char op = popc(&operators);
                if(op == '+')
                    pushf(&operands, op1+op2);
                else if(op == '-')
                    pushf(&operands, op1-op2);
                else if(op == '*')
                    pushf(&operands, op1*op2);
                else if(op == '/')
                    pushf(&operands, op1/op2);
            }
        }
        else
        {
            float val;
            sscanf(token, "%f", &val);
            pushf(&operands, val);
            if(operators.size > 0 && operators.arr[operators.size-1] != '(' && operators.arr[operators.size-1] != ')')
            {
                float op2 = popf(&operands);
                float op1 = popf(&operands);
                char op = popc(&operators);
                if(op == '+')
                    pushf(&operands, op1+op2);
                else if(op == '-')
                    pushf(&operands, op1-op2);
                else if(op == '*')
                    pushf(&operands, op1*op2);
                else if(op == '/')
                    pushf(&operands, op1/op2);
            }
        }
        token = strtok(NULL, " ");
    }
    return operands.arr[0];
}
int main()
{
    char *exp = NULL;
    int sz = 0;
    getline(&exp, &sz, stdin);
    printf("%f\n", evaluateExpression(exp));
    return 0;
}