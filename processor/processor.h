
#pragma once
#include "stack.h"
#include "common.h"


const size_t value_reg = 0;
const size_t value_RAM = 0;
const int size_reg  = 8;
const int size_RAM  = 100;
const int capacity  = 10;
const int error = -666;

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
    bool is_finished;
    int reg[size_reg];
    int RAM[size_RAM];
    struct stack stk;
    struct stack ret_adrr_stk;
};

int processor_constructor(processor* CPU, FILE* file_read);
int processing_code(processor* CPU);
int processor_verify(processor* CPU);
int processor_errors_output(int proc_errors);
int cpu_assert(processor* CPU);
int processor_dump(processor* CPU);



