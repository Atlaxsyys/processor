#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "processor.h"
#include "colour.h"



int main(const int argc,const char* argv[])
{
    processor CPU = {};

    if(argc != 2)
    {
        fprintf(stderr, "file transfer error: %s\n", argv[1]);
        return 1;
    }

    FILE* file_read = fopen(argv[1], "rb");
    if(!file_read)
    {
        fprintf(stderr, "Error open file: %s", argv[1]);
        return 1;
    }

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
    CPU->is_finished = 1;
    memset(CPU->reg, value_reg, size_reg);
    memset(CPU->RAM, value_RAM, size_RAM);
    stack_constructor(&CPU->stk, capacity);
    stack_constructor(&CPU->ret_adrr_stk, capacity);

    proc_assert(CPU);

    return 0;
}

int processing_code(processor* CPU)
{
    proc_assert(CPU);

    
    while(CPU->is_finished)
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

    proc_assert(CPU);

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

int push_cpu(processor* CPU)
{
    proc_assert(CPU);

    stack_push(&CPU->stk, CPU->code[CPU->ip + 1]);    // a->b = (*a).b
    CPU->ip += 2;

    return 0;
}

int math_operation_cpu(processor* CPU, int code_operation)
{
    proc_assert(CPU);

    switch(code_operation)
    {
        case ADD:
        {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
            stack_push(&CPU->stk, number_first + number_second);

            break;
        }
        case SUB:
        {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
            stack_push(&CPU->stk, number_second - number_first);

            break;
        }
        case MUL:
        {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
            stack_push(&CPU->stk, number_first * number_second);

            break;
        }
        case DIV:
        {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
            stack_push(&CPU->stk, number_second / number_first);

            break;
        }
        case SQRT:
        {
            int elem = stack_pop(&CPU->stk);
            int res = (int) sqrt(elem);
            stack_push(&CPU->stk, res);

            break;
        }
        default:
        {

            fprintf(stderr, "UNKWOWN OPERATION: %d", code_operation);
        }
    }

    CPU->ip++;
    
    return 0;
    
}

int out_cpu(processor* CPU)
{
    proc_assert(CPU);

    int deleted_value = stack_pop(&CPU->stk);
    printf("head_stack:  %d\n", deleted_value);
    CPU->ip++;

    return 0;
}

int hlt_cpu(processor* CPU)
{
    proc_assert(CPU);
    
    CPU->ip++;   
    CPU->is_finished = 0;

    return 0;
}

int jump_cpu(processor* CPU, int code_operation)
{
    assert(CPU);

    switch(code_operation)
    {
        case JMP:
        {
            CPU->ip = CPU->code[CPU->ip + 1];

            break;
        }
        case JE:
        {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
        
            if (number_first == number_second)
            {
                CPU->ip = CPU->code[CPU->ip + 1];
            }
            else
            {
                CPU->ip += 2;
            }

            break;
        }
        case JNE:
        {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);
        
            if (number_first != number_second)
            {
                CPU->ip = CPU->code[CPU->ip + 1];
            }
            else
            {
                CPU->ip += 2;
            }

            break;
        }
        case JB:
        {
            int number_first = stack_pop(&CPU->stk);
            int number_second = stack_pop(&CPU->stk);

            if (number_first < number_second)
            {
                CPU->ip = CPU->code[CPU->ip + 1];
            }
            else
            {
                CPU->ip += 2;
            }

            break;
        }
        default:
        {
            fprintf(stderr, "UNKNOWN JUMP_CODE: %d", code_operation);
        }
    }

    return 0;
}

int call_cpu(processor* CPU)
{
    proc_assert(CPU);

    stack_push(&CPU->ret_adrr_stk, CPU->ip + 2);
    CPU->ip = CPU->code[CPU->ip + 1];

    return 0;
}

int ret_cpu(processor* CPU)
{
    proc_assert(CPU);

    int after_call = stack_pop(&CPU->ret_adrr_stk);
    CPU->ip = after_call;

    return 0;
}

int get_index_reg(processor* CPU)
{
    if(CPU->code[CPU->ip + 1] == RAX)
    {
        return RAX;
    }
    else if(CPU->code[CPU->ip + 1] == RBX)
    {
        return RBX;
    }
    else if(CPU->code[CPU->ip + 1] == RCX)
    {
        return RCX;
    }
    else if(CPU->code[CPU->ip + 1] == RDX)
    {
        return RDX;
    }
    
    return error;
}

int push_reg_cpu(processor* CPU)
{
    assert(CPU);
    
    int reg_index = get_index_reg(CPU);
    stack_push(&CPU->stk, CPU->reg[reg_index]);

    CPU->ip += 2;

    return 0;
}


int pop_reg_cpu(processor* CPU, int deleted_number)
{
    assert(CPU);
    
    int reg_index = get_index_reg(CPU);
    CPU->reg[reg_index] = deleted_number;

    CPU->ip += 2;

    return 0;
}

int in_cpu(processor* CPU)
{
    assert(CPU);

    int number = 0;
    scanf("%d", &number);

    stack_push (&CPU->stk, number);

    CPU->ip += 1;

    return 0;
}

int processor_dump(processor* CPU)
{
    proc_assert(CPU);

    fprintf(stderr, YELLOW "______________________________________________________\n" CLEAR);

    fprintf(stderr, RED "CPU->ip = %d\n" CLEAR, CPU->ip);
    for(int i = 0; i < CPU->ip; i++)
    {
        fprintf (stderr, BLUE "CPU->code[%d] = %d\n" CLEAR, i, CPU->code[i]);
    }
    for(int i = 0; i < size_reg; i++)
    {
        fprintf(stderr, PURPLE "CPU->reg[%d] = %d\n" CLEAR, i, CPU->reg[i]);
    }

    int errors = processor_verify(CPU);
    if(errors == 0)
    {
        fprintf(stderr, GREEN "\nCPU IS GOOD\n" CLEAR);
    }

    fprintf(stderr, YELLOW "______________________________________________________\n" CLEAR);

    return 0;
}

const char* converting_to_str(int proc_errors)
{
    switch(proc_errors)
    {
        case CPU_IS_NULL:
            return "CPU_IS_NULL";
        case CPU_CODE_IS_NULL:
            return "CPU_CODE_IS_NULL";
        case CPU_IP_IS_BAD:
            return "CPU_IP_IS_BAD";

        default:
        return "UNKNOWN OPERATION";
    }
}