#pragma once

#include <stdio.h>
#include "common.h"


const int SIZE_COMMANDS = 200;
const int SIZE_LBL = 100;


struct lbl
{
    int labels[SIZE_LBL];
    int addr;
};


int assembler(int* commands, char* commands_buffer, lbl* mtk, long size_file);
// void output_in_convert_commands(int* commands, int pointer, FILE* file_write_second);
// long size_commands(FILE* file_read);
// char* create_buffer(FILE* file_read);
// int n_string(char* commands_buffer, long size_command);
// int push(int* commands, int* pointer, char** commands_buffer);
// int pop(int* commands, int* pointer, char** commands_buffer);
// int math_operation(int* commands, int* pointer, char* command);
// int out(int* commands, int* pointer);
// int hlt(int* commands, int* pointer);
// int call(int* commands, int* pointer, char** commands_buffer, lbl* mtk);
// int ret(int* commands, int* pointer);
// int jump(int* commands, int* pointer, char** commands_buffer, char* command, lbl* mtk);
// int str_to_jmp(char* command);
// int label(char* command, int* pointer, lbl* mtk);
// int push_r(char** commands_buffer, int* commands, int* pointer);
// int pop_r(char** commands_buffer, int* commands, int* pointer);
// int reg(int* commands, int* pointer, char* reg_type);
// int in(int* commands, int* pointer);
int free_asm(int * commands, char* c_buffer);
// int str_to_reg(char* reg_type);
// int str_to_math_op(char* command);
// bool is_math_op(char* command);
// bool is_jump_op(char* command);








