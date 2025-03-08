#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "processor.h"
#include "colour.h"
#include "processor_functions.h"


int main(const int argc,const char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Error: not enough args:\n %s <bin_file>", argv[0]);

        return 1;
    }
    
    FILE* file_read = fopen(argv[1], "rb");

    if(!file_read)
    {
        fprintf(stderr, "Error open file: %s", argv[1]);

        return 1;
    }
    
    processor CPU = {};

    processor_constructor(&CPU, file_read);
    fclose(file_read);
    processing_code(&CPU);
    processor_dump(&CPU);
    free(CPU.code);
    stack_destroy(&CPU.stk);
    stack_destroy(&CPU.ret_adrr_stk);
}

int processor_constructor(processor* CPU, FILE* file_read)
{
    assert(CPU);

    int pointer = 0;
    
    fread(&pointer, sizeof(int), 1, file_read);

    CPU->code = (int* )calloc((size_t)pointer, sizeof(int));

    fread(CPU->code, sizeof(int),(size_t) pointer, file_read);

    CPU->ip = 0;
    CPU->is_finished = false;

    memset(CPU->reg, value_reg, size_reg);
    memset(CPU->RAM, value_RAM, size_RAM);
    stack_constructor(&CPU->stk, capacity);
    stack_constructor(&CPU->ret_adrr_stk, capacity);

    cpu_assert(CPU);

    return 0;
}

int processing_code(processor* CPU)
{
    cpu_assert(CPU);

    while(!CPU->is_finished)
    {
        switch(CPU->code[CPU->ip])
        {
            case PUSH:
            {
                push_cpu(CPU);

                break;
            }
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case SQRT:
            {
                math_operation_cpu(CPU, CPU->code[CPU->ip]);

                break;
            }
            case OUT:
            {
                out_cpu(CPU);

                break;
            }
            case HLT:
            {
                hlt_cpu(CPU);

                break;
            }
            case PUSH_R:
            {
                push_reg_cpu(CPU);

                break;
            }
            case POP_R:
            {
                int deleted_number = stack_pop(&CPU->stk);
                pop_reg_cpu(CPU, deleted_number);

                break;
            }
            case JMP:
            case JE:
            case JNE:
            case JB:
            {
                jump_cpu(CPU, CPU->code[CPU->ip]);

                break;
            }
            case CALL:
            {
                call_cpu(CPU);

                break;
            }
            case RET:
            {
                ret_cpu(CPU);

                break;
            }
            case IN:
            {
                in_cpu(CPU);

                break;
            }
            default:
            {
                fprintf(stderr, "UNKNOWN COMMAND: %d", CPU->code[CPU->ip]);
            }
        }
    }

    cpu_assert(CPU);

    return 0;
}

int processor_verify(processor* CPU)
{
    assert(CPU);

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

    fprintf(stderr, "ERRORS: \n\n");

    fprintf(stderr, "%s", converting_to_str(proc_errors));

    return 0;
}

int cpu_assert(processor* CPU)
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