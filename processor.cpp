#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "asm_proc.h"




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
                printf("head_stack:  %d\n", deleted_value);
                i++;

                break;
            }
            case HLT:
            {
                i++;   
                tmp = 0;

                break;
            }
            case PUSH_R:
            {
                if(CPU->code[i + 1] == RAX)
                {
                    stack_push(&CPU->stk, CPU->reg[0]);
                    i += 2;
                }

                else if(CPU->code[i + 1] == RBX)
                {
                    stack_push(&CPU->stk, CPU->reg[1]);
                    i += 2;
                }

                break;
                
            }
                case POP_R:
                {
                    int deleted_number = stack_pop(&CPU->stk);
                    
                    if(CPU->code[i + 1] == RAX)
                    {
                        CPU->reg[0] = deleted_number;
                        i += 2;
                    }
                    else if(CPU->code[i + 1] == RBX)
                    {
                        CPU->reg[1] = deleted_number;
                        i += 2;
                    }
                    break;
                }
                case JMP:
                {
                    i = CPU->code[i + 1];

                    break;
                }
                default:
                fprintf(stderr, "UNKNOWN COMMAND:   %d", CPU->code[i]);
                abort();
        }
    }

    return 0;
}