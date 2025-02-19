#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>


const int value_reg = 0;
const int value_RAM = 0;
const int size_reg = 8;
const int size_RAM = 100;

struct processor 
{
    int* code;
    int ip;
    int reg[size_reg];
    int RAM[size_RAM];
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


int processor_constructor(processor* CPU, FILE* file_read);
int processing_code(processor* CPU);





int main(int argc, char* argv[])
{
    processor CPU = {};


    FILE* file_read = fopen(argv[1], "rb");

    processor_constructor(&CPU, file_read);

    fclose(file_read);


    processing_code(&CPU);
    

}

int processor_constructor(processor* CPU, FILE* file_read)
{
    assert(CPU);

    if(!file_read)
    {
        fprintf(stderr, "Error open file");
        abort();
    }

    int pointer = 0;
    
    fread(&pointer, sizeof(int), 1, file_read);

    

    CPU->code = (int* )calloc(pointer, sizeof(int));

    fread(CPU->code, sizeof(int), pointer, file_read);

    CPU->ip = 0;
    memset(CPU->reg, value_reg, size_reg);
    memset(CPU->RAM, value_RAM, size_RAM);
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
    assert(CPU);

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