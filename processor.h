
#pragma once
#include "stack.h"


const int value_reg = 0;
const int value_RAM = 0;
const int size_reg  = 8;
const int size_RAM  = 100;
const int capacity  = 5;

enum processor_errors
{
    CPU_IS_NULL      = 1 << 0,
    CPU_CODE_IS_NULL = 1 << 1,
    CPU_IP_IS_BAD    = 1 << 2,
};


struct processor 
{
    int* code;
    int ip;
    int reg[size_reg];
    int RAM[size_RAM];
    struct stack stk;
    struct stack ret_adrr_stk;
};

int processor_constructor(processor* CPU, FILE* file_read);
int processing_code(processor* CPU);
int processor_verify(processor* CPU);
int processor_errors_output(int proc_errors);
int proc_assert(processor* CPU);
int push_cpu(processor* CPU);
int math_operation_cpu(processor* CPU, enum commands operation);
int out_cpu(processor* CPU);
int hlt_cpu(processor* CPU, int* tmp);
int jmp_cpu(processor* CPU);
int call_cpu(processor* CPU);
int ret_cpu(processor* CPU);
int rax_cpu(processor* CPU);
int rbx_cpu(processor* CPU);
int processor_dump(processor* CPU);
const char* converting_to_str(int proc_errors);







