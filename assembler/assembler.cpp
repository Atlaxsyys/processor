#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "assembler.h"
#include "assembler_functions.h"
#include "file_data.h"


int main(const int argc, const char* argv[])
{
    lbl mtk = {};

    int* commands = (int*)calloc(SIZE_COMMANDS, sizeof(int));

    if(argc != 3)
    {
        fprintf(stderr, "Error: not enough args:\n %s <asm_file> <bin_file>", argv[0]);

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

    fclose(file_read);

    int pointer = assembler(commands, c_buffer, &mtk, size_file);
    assembler(commands, c_buffer, &mtk, size_file);  

    FILE* file_write = fopen(argv[2], "wb");
    
    if(!file_write)
    {
        fprintf(stderr, "Error open file: %s", argv[2]);
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

        else if(is_math_op(command))
        {
            math_operation(commands, &pointer, command);
        }

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
            push_r(&commands_buffer, commands, &pointer);
        }

        else if(strcmp(command, "pop_r") == 0)
        {
            pop_r(&commands_buffer, commands, &pointer);
        }

        else if (strcmp(command, "call") == 0)
        {
            call(commands, &pointer, &commands_buffer, mtk);
        }

        else if(strcmp(command, "in") == 0)
        {
            in(commands, &pointer);
        }

        else if(is_jump_op(command))
        {
            jump(commands, &pointer, &commands_buffer, command, mtk);
        }

        counter++;
        number_of_operation++;
    }

    commands[0] = pointer - 1;

    return pointer;
}

int free_asm(int * commands, char* c_buffer)
{
    assert(commands);
    assert(c_buffer);

    free(commands);
    free(c_buffer);

    return 0;
}

