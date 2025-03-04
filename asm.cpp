#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "asm.h"



int main(const int argc, const char* argv[])
{
    lbl mtk = {};

    int* commands = (int*)calloc(SIZE_COMMANDS, sizeof(int));

    if(argc != 3)
    {
        fprintf(stderr, "file transfer error: %s and %s", argv[1], argv[2]);
        return 1;
    }

    FILE* file_read = fopen(argv[1], "rb");
    if(!file_read)
    {
        fprintf(stderr, "Error open file: %s", argv[1]);
        return 1;
    }
    char* c_buffer = create_buffer(file_read);
    long size_file = size_commands(file_read);


    int pointer = assembler(commands, c_buffer, &mtk, size_file);
    assembler(commands, c_buffer, &mtk, size_file);  


    fclose(file_read);

    FILE* file_write = fopen(argv[2], "wb");
    if(!file_write)
    {
        fprintf(stderr, "Error open file: converted_commands.bin");
        return 1;
    }

    output_in_convert_commands(commands, pointer, file_write);

    fclose(file_write);

    free_asm(commands, c_buffer);
}



int assembler(int* commands, char* commands_buffer, lbl* mtk, long size_file) //TODO cut back
{

    assert(commands);
    assert(commands_buffer);
    assert(mtk);

    char command[10] = {};
    int counter = 0;
    int pointer = 1;
    int number_of_operation = 0;
    int number_string = n_string(commands_buffer, size_file);


    int pointer_scanf = 0;
    while(counter <  number_string)
    {
        sscanf(commands_buffer, "%6s %n", command, &pointer_scanf);
        commands_buffer = commands_buffer + pointer_scanf;

        if(strchr(command, ':') != NULL)
        {
            label(command, &pointer, mtk);
        }
        
        if(strcmp(command, "push") == 0)
        {
            push(commands, &pointer, &commands_buffer);
        }

        else if (strcmp(command, "ret") == 0)
        {
            ret(commands, &pointer);
        }

        else if(strcmp(command, "pop") == 0)
        {
            pop(commands, &pointer, &commands_buffer);
        }

        math_operation(commands, &pointer, command);

        if(strcmp(command, "out") == 0)
        {
            out(commands, &pointer);
        }

        else if(strcmp(command, "hlt") == 0)
        {
            hlt(commands, &pointer);

        }
        else if(strcmp(command, "push_r") == 0)
        {
            push_r(commands, &pointer);
            char reg_type[10] = {};
            sscanf(commands_buffer, "%3s %n", reg_type, &pointer_scanf);
            commands_buffer = commands_buffer + pointer_scanf;
            
            reg(commands, &pointer, reg_type);
        }

        else if(strcmp(command, "pop_r") == 0)
        {
            pop_r(commands, &pointer);
            char reg_type[10] = {};
            sscanf(commands_buffer, "%3s %n", reg_type, &pointer_scanf);
            commands_buffer = commands_buffer + pointer_scanf;

            reg(commands, &pointer, reg_type);
        }

        else if (strcmp(command, "call") == 0)
        {
            call(commands, &pointer, &commands_buffer, mtk);
        }
        else if(strcmp(command, "in") == 0)
        {
            in(commands, &pointer);
        }
        
        jump(commands, &pointer, &commands_buffer, command, mtk);


        counter++;
        number_of_operation++;
    }
    commands[0] = pointer - 1;

    return pointer;
}

void output_in_convert_commands(int* commands, int pointer, FILE* file_write)
{
    assert(commands);
    assert(file_write);

    int number_of_write = fwrite(commands, sizeof(int),(size_t) pointer, file_write);
    
    if (number_of_write != pointer)
    {
        fprintf(stderr, "file recording error!");
    }

}

long size_commands(FILE* file_read)
{
    assert(file_read);

    fseek(file_read, 0, SEEK_END);
    long size_file = ftell(file_read);
    fseek(file_read, 0, SEEK_SET);

    return size_file;
}

char* create_buffer(FILE* file_read)
{
    assert(file_read);


    long size_file = size_commands(file_read);

    char* commands_buffer = (char*) calloc((size_t) size_file + 1, sizeof(char));

    fread(commands_buffer, sizeof(char), (size_t) size_file + 1, file_read);
    rewind(file_read);

    return commands_buffer;
}

int n_string(char* commands_buffer, long size_command)
{
    assert(commands_buffer);

    int counter_string = 0;
    for (int i = 0; i < size_command; i++)
    {
        if (commands_buffer[i] == '\n')
        {
            counter_string++;
        }
    }

    return counter_string + 1;
}

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
int str_to_math_op(char* command)
{
    assert(command);
    if(strcmp(command, "add") == 0)
    {
        return ADD;
    }
    else if(strcmp(command, "sub") == 0)
    {
        return SUB;
    }
    else if(strcmp(command, "mul") == 0)
    {
        return MUL;
    }
    else if(strcmp(command, "div") == 0)
    {
        return DIV;
    }
    else if(strcmp(command, "sqrt") == 0)
    {
        return SQRT;
    }
    
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

int str_to_jmp(char* command)
{
        if(strcmp(command, "jmp") == 0)
        {
            return JMP;
        }
        else if(strcmp(command, "je") == 0)
        {
            return JE;
        }
        else if(strcmp(command, "jne") == 0)
        {
            return JNE;
        }
        else if(strcmp(command, "jb") == 0)
        {
            return JB;
        }

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

    if(strcmp(reg_type, "rax") == 0)
    {
        return RAX;
    }
    else if(strcmp(reg_type, "rbx") == 0)
    {
        return RBX;
    }
    else if(strcmp(reg_type, "rcx") == 0)
    {
        return RCX;
    }
    else if(strcmp(reg_type, "rdx") == 0)
    {
        return RDX;
    }

    return -1;
}

int push_r(int* commands, int* pointer)
{
    assert(commands);

    commands[(*pointer)] = PUSH_R;

    return 0;
}

int pop_r(int* commands, int* pointer)
{
    assert(commands);
    
    commands[(*pointer)] = POP_R;

    return 0;
}

int in(int* commands, int* pointer)
{
    assert(commands);

    commands[*pointer] = IN;
    (*pointer)++;

    return 0;
}

int free_asm(int * commands, char* c_buffer)
{
    assert(commands);
    assert(c_buffer);

    free(commands);
    free(c_buffer);

    return 0;
}

