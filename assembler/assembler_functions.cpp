#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "assembler.h"
#include "assembler_functions.h"

int push(int* commands, int* pointer, char** commands_buffer)
{
    assert(commands);
    assert(pointer);
    assert(commands_buffer);

    int pointer_scanf = 0;
    commands[*pointer] = PUSH;

    sscanf(*commands_buffer, "%d %n", &commands[*pointer + 1], &pointer_scanf);

    (*pointer) += 2;
    *commands_buffer = *commands_buffer + pointer_scanf;

    return 0;
}

int pop(int* commands, int* pointer, char** commands_buffer)
{
    assert(commands);
    assert(pointer);
    assert(commands_buffer);

    int pointer_scanf = 0;
    commands[*pointer] = POP;

    sscanf(*commands_buffer, "%d %n", &commands[*pointer + 1], &pointer_scanf);

    (*pointer) += 2;
    *commands_buffer = *commands_buffer + pointer_scanf;

    return 0;
}

bool is_math_op(char* command)
{
    assert(command);

         if(strcmp(command, "add" ) == 0)  return true;
    else if(strcmp(command, "sub" ) == 0)  return true;
    else if(strcmp(command, "mul" ) == 0)  return true;
    else if(strcmp(command, "div" ) == 0)  return true;
    else if(strcmp(command, "sqrt") == 0)  return true;

    return false;
}

int str_to_math_op(char* command)
{
    assert(command);

         if(strcmp(command, "add" ) == 0)  return ADD;
    else if(strcmp(command, "sub" ) == 0)  return SUB;
    else if(strcmp(command, "mul" ) == 0)  return MUL;
    else if(strcmp(command, "div" ) == 0)  return DIV;
    else if(strcmp(command, "sqrt") == 0)  return SQRT;
    
    return 0;
}

int math_operation(int* commands, int* pointer, char* command)
{
    assert(commands);
    assert(pointer);
    assert(command);

    commands[*pointer] = str_to_math_op(command);

    if (commands[*pointer] != 0)
        (*pointer)++;

    return 0;
}

int out(int* commands, int* pointer)
{
    assert(commands);
    assert(pointer);

    commands[*pointer] = OUT;
    (*pointer)++;

    return 0;
}

int hlt(int* commands, int* pointer)
{
    assert(commands);
    assert(pointer);

    commands[*pointer] = HLT;
    (*pointer)++;

    return 0;
}

int call(int* commands, int* pointer, char** commands_buffer, lbl* mtk)
{
    assert(commands);
    assert(commands_buffer);
    assert(mtk);

    int pointer_scanf = 0;
    commands[*pointer] = CALL;

    int func = 0;
    sscanf(*commands_buffer, "%d: %n", &func, &pointer_scanf);
    commands[*pointer + 1] = mtk->labels[func];

    (*pointer) += 2;
    *commands_buffer = *commands_buffer + pointer_scanf;

    return 0;
}

int ret(int* commands, int* pointer)
{
    assert(commands);
    assert(pointer);

    commands[*pointer] = RET;
    (*pointer)++;

    return 0;
}

bool is_jump_op(char* command)
{
        if(strcmp(command, "jmp" ) == 0) return true;
    else if(strcmp(command, "je" ) == 0) return true;
    else if(strcmp(command, "jne") == 0) return true;
    else if(strcmp(command, "jb" ) == 0) return true;
    else if(strcmp(command, "jbe") == 0) return true;
    else if(strcmp(command, "ja" ) == 0) return true;
    else if(strcmp(command, "jae") == 0) return true;

    return false;
}

int str_to_jmp(char* command)
{
        if(strcmp(command, "jmp" ) == 0) return JMP;
    else if(strcmp(command, "je" ) == 0) return JE;
    else if(strcmp(command, "jne") == 0) return JNE;
    else if(strcmp(command, "jb" ) == 0) return JB;
    else if(strcmp(command, "jbe") == 0) return JBE;
    else if(strcmp(command, "ja" ) == 0) return JA;
    else if(strcmp(command, "jae") == 0) return JAE;

    return 0;
}

int jump(int* commands, int* pointer, char** commands_buffer, char* command, lbl* mtk)
{
    assert(commands);
    assert(commands_buffer);
    assert(command);
    assert(mtk);

    int pointer_scanf = 0;
    commands[*pointer] = str_to_jmp(command);
    
    if (commands[*pointer] != 0)
    {
        int label = 0;
        sscanf(*commands_buffer, "%d: %n", &label, &pointer_scanf);
        commands[*pointer + 1] = mtk->labels[label];

        (*pointer) += 2;
        *commands_buffer = *commands_buffer + pointer_scanf;
    }

    return 0;
}

int label(char* command, int* pointer, lbl* mtk)
{
    assert(command);
    assert(mtk);

    int number_mtk = 0;
    sscanf(command, "%d:", &number_mtk);
    mtk->labels[number_mtk] = (*pointer) - 1;  // because commands[0] = number of operations

    return 0;
}

int reg(int* commands, int* pointer, char* reg_type)
{
    assert(commands);
    assert(pointer);
    assert(reg_type);

    commands[(*pointer) + 1] = str_to_reg(reg_type);

    if (commands[(*pointer) + 1] != -1)
        (*pointer) += 2;

    return 0;
}

int str_to_reg(char* reg_type)
{
    assert(reg_type);

         if(strcmp(reg_type, "rax") == 0) return RAX;
    else if(strcmp(reg_type, "rbx") == 0) return RBX;
    else if(strcmp(reg_type, "rcx") == 0) return RCX;
    else if(strcmp(reg_type, "rdx") == 0) return RDX;

    return -1;
}

int push_r(char** commands_buffer, int* commands, int* pointer)
{
    assert(commands);

    commands[(*pointer)] = PUSH_R;

    char reg_type[10] = {};
    int pointer_scanf = 0;

    sscanf(*commands_buffer, "%3s %n", reg_type, &pointer_scanf);
    *commands_buffer = *commands_buffer + pointer_scanf;

    reg(commands, pointer, reg_type);

    return 0;
}

int pop_r(char** commands_buffer, int* commands, int* pointer)
{
    assert(commands);
    
    commands[(*pointer)] = POP_R;

    char reg_type[10] = {};
    int pointer_scanf = 0;
    
    sscanf(*commands_buffer, "%3s %n", reg_type, &pointer_scanf);
    *commands_buffer = *commands_buffer + pointer_scanf;

    reg(commands, pointer, reg_type);

    return 0;
}

int in(int* commands, int* pointer)
{
    assert(commands);

    commands[*pointer] = IN;
    (*pointer)++;

    return 0;
}