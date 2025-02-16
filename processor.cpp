#include "stack.h"
#include <stdlib.h>

struct processor 
{
    int* code;
    int ip;
    int reg[8];
    int RAM[100];
    struct stack stk;
};

enum commands
{
    HLT = -1,
    OUT = 0,
    PUSH = 1,
    ADD = 3,
    SUB = 4,
};

int processor_constructor(processor* CPU);
int processing_code(processor* CPU);





int main()
{
    processor CPU = {};



    processor_constructor(&CPU);
    processing_code(&CPU);
    

}

int processor_constructor(processor* CPU)
{
    FILE* file_read = fopen("converted_commands.bin", "rb");

    int pointer = 0;
    
    fread(&pointer, sizeof(int), 1, file_read);

    

    CPU->code = (int* )calloc(pointer, sizeof(int));

    fread(CPU->code, sizeof(int), pointer, file_read);

    fclose(file_read);

    CPU->ip = 0;
    CPU->reg[8] = {};
    CPU->RAM[100] = {};
    stack_constructor(&CPU->stk, 5);

    printf("%d", pointer);

    // for(int i = 0; i < pointer; i++)
    // {
    //     fprintf(stderr, "%d\n", CPU->code[i]);
    // }

    return 0;
}

int processing_code(processor* CPU)
{
    int i = 0;
    int tmp = 1;
    while(tmp)
    {
        switch(CPU->code[i])
        {
        case PUSH:
            {
            stack_push(&CPU->stk, CPU->code[i + 1]);    // a->b = (*a).b
            i += 2;
            break;
            }
        case ADD:
            {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
            stack_push(&CPU->stk, number_first + number_second);
            i++;

            break;
            }
        case SUB:
            {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
            stack_push(&CPU->stk, number_second - number_first);
            i++;

            break;
            }
        case OUT:
            {
            int deleted_value = stack_pop(&CPU->stk);
            printf("head_stack:  %d", deleted_value);
            i++;

            break;
            }
        case HLT:
            {
            i++;   
            tmp = 0;

            break;
            }
        }
    }

    return 0;
}