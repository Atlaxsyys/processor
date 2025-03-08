#pragma once

#include "assembler.h"

int push(int* commands, int* pointer, char** commands_buffer);
int pop(int* commands, int* pointer, char** commands_buffer);
int math_operation(int* commands, int* pointer, char* command);
int out(int* commands, int* pointer);
int hlt(int* commands, int* pointer);
int call(int* commands, int* pointer, char** commands_buffer, lbl* mtk);
int ret(int* commands, int* pointer);
int jump(int* commands, int* pointer, char** commands_buffer, char* command, lbl* mtk);
int str_to_jmp(char* command);
int label(char* command, int* pointer, lbl* mtk);
int push_r(char** commands_buffer, int* commands, int* pointer);
int pop_r(char** commands_buffer, int* commands, int* pointer);
int reg(int* commands, int* pointer, char* reg_type);
int in(int* commands, int* pointer);
int str_to_reg(char* reg_type);
int str_to_math_op(char* command);
bool is_math_op(char* command);
bool is_jump_op(char* command);
