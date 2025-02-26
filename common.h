#pragma once

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
    MUL = 5,
    DIV = 6,
    PUSH_R = 11,
    POP_R = 12,
    JMP = 13,
    RAX = 20,
    RBX = 21,
    CALL = 30,
    RET = 31,
};



// const int value_reg = 0;
// const int value_RAM = 0;
// const int size_reg = 8;
// const int size_RAM = 100;
// const int SIZE_COMMANDS = 200;
// const int SIZE_LBL = 100;

// struct processor 
// {
//     int* code;
//     int ip;
//     int reg[size_reg];
//     int RAM[size_RAM];
//     struct stack stk;
// };

// struct lbl
// {
//     char labels[SIZE_LBL];
//     int addr;
// };


// int assembler(int* commands, FILE* file_read, lbl* mtk);
// void printf_in_convert_commands(int* commands, int pointer, FILE* file_write_second);


// int processor_constructor(processor* CPU, FILE* file_read);
// int processing_code(processor* CPU);
