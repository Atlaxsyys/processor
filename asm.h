#pragma once

#include <stdio.h>

const int SIZE_COMMANDS = 200;
const int SIZE_LBL = 100;


struct lbl
{
    char labels[SIZE_LBL];
    int addr;
};


int assembler(int* commands, char* commands_buffer, lbl* mtk, long size_file);
void output_in_convert_commands(int* commands, int pointer, FILE* file_write_second);
long size_commands(FILE* file_read);
char* create_buffer(FILE* file_read);
int n_string(char* commands_buffer, long size_command);
int push(int* commands, int* pointer, char** commands_buffer);
int pop(int* commands, int* pointer, char** commands_buffer);
int math_operation(int* commands, int* pointer, enum commands operation);
int out(int* commands, int* pointer);
int hlt(int* commands, int* pointer);
int call(int* commands, int* pointer, char** commands_buffer, lbl* mtk);
int ret(int* commands, int* pointer);
int jmp(int* commands, int* pointer, char** commands_buffer, lbl* mtk);
int label(char* command, int* pointer, lbl* mtk);
int rax(int* commands, int* pointer);
int rbx(int* commands, int* pointer);
int push_r(int* commands, int* pointer);
int pop_r(int* commands, int* pointer);







