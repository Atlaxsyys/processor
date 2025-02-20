#ifndef ASM_PROC_H_
#define ASM_PROC_H_

#include <stdio.h>

#include "stack.h"

enum commands
{
    HLT = -1,
    OUT = 0,
    PUSH = 1,
    POP = 2,
    ADD = 3,
    SUB = 4,
};



const int value_reg = 0;
const int value_RAM = 0;
const int size_reg = 8;
const int size_RAM = 100;
const int SIZE_COMMANDS = 200;

struct processor 
{
    int* code;
    int ip;
    int reg[size_reg];
    int RAM[size_RAM];
    struct stack stk;
};


int assembler(int* commands, FILE* file_read);
void printf_in_convert_commands(int* commands, int pointer, FILE* file_write_second);


int processor_constructor(processor* CPU, FILE* file_read);
int processing_code(processor* CPU);

#endif // ASM_PROC_H_