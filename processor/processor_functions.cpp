#include <assert.h>
#include <math.h>

#include "colour.h"
#include "processor_functions.h"

int push_cpu(processor* CPU)
{
    cpu_assert(CPU);

    stack_push(&CPU->stk, CPU->code[CPU->ip + 1]);
    CPU->ip += 2;

    return 0;
}

int binary_operation(processor* CPU, int (*operation)(int, int))
{
    int number_first = stack_pop(&CPU->stk);
    int number_second = stack_pop(&CPU->stk);
    stack_push(&CPU->stk, operation(number_second, number_first));

    return 0;
}

int add(int a, int b)    { return a + b; }
int sub(int a, int b)    { return a - b; }
int mul(int a, int b)    { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

int math_operation_cpu(processor* CPU, int code_operation)
{
    cpu_assert(CPU);

    switch (code_operation)
    {
        case ADD:
        {
            binary_operation(CPU, add);
            break;
        }
        case SUB:
        {
            binary_operation(CPU, sub);
            break;
        }
        case MUL:
        {
            binary_operation(CPU, mul);
            break;
        }
        case DIV:
        {
            binary_operation(CPU, divide);
            break;
        }
        case SQRT:
        {
            int elem = stack_pop(&CPU->stk);
            stack_push(&CPU->stk, (int) sqrt(elem));
            break;
        }
        default:
        {
            fprintf(stderr, "UNKNOWN OPERATION: %d\n", code_operation);
        }
    }

    CPU->ip++;

    return 0;
}

int out_cpu(processor* CPU)
{
    cpu_assert(CPU);

    int deleted_value = stack_pop(&CPU->stk);
    printf("head_stack:  %d\n", deleted_value);

    CPU->ip++;

    return 0;
}

int hlt_cpu(processor* CPU)
{
    cpu_assert(CPU);
    
    CPU->ip++;   
    CPU->is_finished = true;

    return 0;
}

int conditional_jump(processor* CPU, int (*cmp)(int, int))
{
    cpu_assert(CPU);

    int number_first = stack_pop(&CPU->stk);
    int number_second = stack_pop(&CPU->stk);

    if (cmp(number_second, number_first))
    {
        CPU->ip = CPU->code[CPU->ip + 1];
    }
    else
    {
        CPU->ip += 2;
    }

    return 0;
}

int is_equal(int a, int b)          { return a == b; }
int is_not_equal(int a, int b)      { return a != b; }
int is_below(int a, int b)          { return b < a;  }
int is_below_or_equal(int a, int b) { return b <= a; }
int is_above(int a, int b)          { return b > a;  }
int is_above_or_equal(int a, int b) { return b >= a; } 


int jump_cpu(processor* CPU, int code_operation)
{
    cpu_assert(CPU);

    switch (code_operation)
    {
        case JMP:
        {
            CPU->ip = CPU->code[CPU->ip + 1];
            break;
        }
        case JE:
        {
            conditional_jump(CPU, is_equal);
            break;
        }
        case JNE:
        {
            conditional_jump(CPU, is_not_equal);
            break;
        }
        case JB:
        {
            conditional_jump(CPU, is_below);
            break;
        }
        case JBE:
        {
            conditional_jump(CPU, is_below_or_equal);
            break;
        }
        case JA:
        {
            conditional_jump(CPU, is_above);
            break;
        }
        case JAE:
        {
            conditional_jump(CPU, is_above_or_equal);
            break;
        }
        default:
        {
            fprintf(stderr, "UNKNOWN JUMP_CODE: %d\n", code_operation);
        }
    }

    return 0;
}


int call_cpu(processor* CPU)
{
    cpu_assert(CPU);

    stack_push(&CPU->ret_adrr_stk, CPU->ip + 2);
    CPU->ip = CPU->code[CPU->ip + 1];

    return 0;
}

int ret_cpu(processor* CPU)
{
    cpu_assert(CPU);

    int after_call = stack_pop(&CPU->ret_adrr_stk);
    CPU->ip = after_call;

    return 0;
}

int get_index_reg(processor* CPU)
{
    cpu_assert(CPU);

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
    cpu_assert(CPU);
    
    int reg_index = get_index_reg(CPU);
    stack_push(&CPU->stk, CPU->reg[reg_index]);

    CPU->ip += 2;

    return 0;
}

int pop_reg_cpu(processor* CPU, int deleted_number)
{
    cpu_assert(CPU);
    
    int reg_index = get_index_reg(CPU);
    CPU->reg[reg_index] = deleted_number;

    CPU->ip += 2;

    return 0;
}

int in_cpu(processor* CPU)
{
    cpu_assert(CPU);

    int number = 0;
    scanf("%d", &number);

    stack_push (&CPU->stk, number);

    CPU->ip += 1;

    return 0;
}

int processor_dump(processor* CPU)
{
    cpu_assert(CPU);

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