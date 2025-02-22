#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "processor.h"




int main(const int argc,const char* argv[])
{
    processor CPU = {};


    FILE* file_read = fopen(argv[1], "rb");
    if(!file_read)
    {
        fprintf(stderr, "Error open file");
        abort();
    }

    processor_constructor(&CPU, file_read);

    fclose(file_read);

    processing_code(&CPU);
    

}

int processor_constructor(processor* CPU, FILE* file_read)
{
    assert(CPU);

    int pointer = 0;
    
    fread(&pointer, sizeof(int), 1, file_read);

    

    CPU->code = (int* )calloc(pointer, sizeof(int));

    fread(CPU->code, sizeof(int), pointer, file_read);

    CPU->ip = 0;
    memset(CPU->reg, value_reg, size_reg);
    memset(CPU->RAM, value_RAM, size_RAM);
    stack_constructor(&CPU->stk, 5);

    proc_assert(CPU);
    return 0;
}

int processing_code(processor* CPU)
{
    proc_assert(CPU);

    int tmp = 1;
    
    while(tmp)
    {
        switch(CPU->code[CPU->ip])
        {
            
            case PUSH:
            {
                stack_push(&CPU->stk, CPU->code[CPU->ip + 1]);    // a->b = (*a).b
                CPU->ip += 2;
                break;
            }
            case ADD:
            {
                int number_first = stack_pop(&CPU->stk);
                int number_second = stack_pop(&CPU->stk);
                stack_push(&CPU->stk, number_first + number_second);
                CPU->ip++;
                break;
            }
            case SUB:
            {
                int number_first = stack_pop(&CPU->stk);
                int number_second = stack_pop(&CPU->stk);
                stack_push(&CPU->stk, number_second - number_first);
                CPU->ip++;

                break;
            }
            case OUT:
            {
                int deleted_value = stack_pop(&CPU->stk);
                printf("head_stack:  %d\n", deleted_value);
                CPU->ip++;

                break;
            }
            case HLT:
            {
                CPU->ip++;   
                tmp = 0;

                break;
            }
            case PUSH_R:
            {
                if(CPU->code[CPU->ip + 1] == RAX)
                {
                    stack_push(&CPU->stk, CPU->reg[0]);
                    CPU->ip += 2;
                }

                else if(CPU->code[CPU->ip + 1] == RBX)
                {
                    stack_push(&CPU->stk, CPU->reg[1]);
                    CPU->ip += 2;
                }

                break;
                
            }
            case POP_R:
            {
                int deleted_number = stack_pop(&CPU->stk);
                
                if(CPU->code[CPU->ip + 1] == RAX)
                {
                    CPU->reg[0] = deleted_number;
                    CPU->ip += 2;
                }
                else if(CPU->code[CPU->ip + 1] == RBX)
                {
                    CPU->reg[1] = deleted_number;
                    CPU->ip += 2;
                }
                break;
            }
            case JMP:
            {
                CPU->ip = CPU->code[CPU->ip + 1];

                break;
            }
            default:

            fprintf(stderr, "UNKNOWN COMMAND:   %d", CPU->code[CPU->ip]);
            abort();
        }
    }

    proc_assert(CPU);

    return 0;
}

int processor_verify(processor* CPU)
{
    int proc_errors = 0;

    if(CPU == NULL)
    {
        proc_errors |= CPU_IS_NULL;
    }

    if(CPU->code == NULL)
    {
        proc_errors |= CPU_CODE_IS_NULL;
    }
    if (CPU->ip < 0)
    {
        proc_errors |= CPU_IP_IS_BAD;
    }

    return proc_errors;
}

int processor_errors_output(int proc_errors)
{
    convert_to_binary(proc_errors);

    printf("ERRORS: \n\n");

    switch(proc_errors)
    {
        case CPU_CODE_IS_NULL:
        {
            fprintf(stderr, "*CPU_CODE_IS_NULL\n");
            break;
        }
        case CPU_IS_NULL:
        {
            fprintf(stderr, "*CPU_IS_NULL\n");
            break;
        }
        case CPU_IP_IS_BAD:
        {
            fprintf(stderr, "*CPU_IP_IS_BAD\n");
            break;
        }
        default:
            fprintf(stderr, "UNKNOWN ERRORS");
    }

    return 0;

}

int proc_assert(processor* CPU)
{
    assert(CPU);

    int proc_errors = processor_verify(CPU);

    if(proc_errors != 0)
    {
        processor_errors_output(proc_errors);
        abort();
    }

    return 0;
}