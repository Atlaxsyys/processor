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
        fprintf(stderr, "file transfer error: commands.txt and converted_commands.bin");
    }

    FILE* file_read = fopen(argv[1], "r");
    if(!file_read)
    {
        fprintf(stderr, "Error open file: commands.txt");
        exit(1);
    }
    // FIXME читай весь файл в буффер и проходись по нему
    char* c_buffer = create_buffer(file_read);
    long size_file = size_commands(file_read);


    int pointer = assembler(commands, c_buffer, &mtk, size_file);
    assembler(commands, c_buffer, &mtk, size_file);  


    fclose(file_read);

    FILE* file_write = fopen(argv[2], "wb");
    if(!file_write)
    {
        fprintf(stderr, "Error open file: converted_commands.bin");
        exit(1);
    }

    output_in_convert_commands(commands, pointer, file_write);

    fclose(file_write);
}



int assembler(int* commands, char* commands_buffer, lbl* mtk, long size_file) //TODO cut back
{
    assert(commands);
    assert(mtk);

    char command[6];
    int counter = 0;
    int pointer = 1;
    int number_of_operation = 0;
    int number_string = n_string(commands_buffer, size_file);


    int pointer_scanf = 0;
    while(counter < number_string)
    {
        int n = 0;
        sscanf(commands_buffer, "%s %n", command, &pointer_scanf);
        commands_buffer = commands_buffer + pointer_scanf;

        if(strchr(command, ':') != NULL)
        {
            int number_mtk = 0;
            sscanf(command, "%d:", &number_mtk);
            mtk->labels[number_mtk] = pointer - 1;  // because commands[0] = number of operations
        }
        
        if(strcmp(command, "push") == 0)
        {
            // commands[pointer] = PUSH;
            // sscanf(commands_buffer, "%d %n", &commands[pointer + 1], &pointer_scanf);
            // commands_buffer = commands_buffer + pointer_scanf;

            // pointer += 2;
            push(commands, &pointer, &commands_buffer);
            // fprintf(stderr, "pointer = %d ", pointer);
        }

        else if(strcmp(command, "pop") == 0)
        {
            // commands[pointer] = POP;
            // sscanf(commands_buffer, "%d %n", &commands[pointer + 1], &pointer_scanf);
            // commands_buffer = commands_buffer + pointer_scanf;

            // pointer += 2;
            pop(commands, &pointer, &commands_buffer);

        }

        else if(strcmp(command, "add") == 0)
        {
            math_operation(commands, &pointer, ADD);
        }

        else if(strcmp(command, "sub") == 0)
        {
            math_operation(commands, &pointer, SUB);
        }
        else if(strcmp(command, "mul") == 0)
        {
            math_operation(commands, &pointer, MUL);
        }
        else if(strcmp(command, "div") == 0)
        {
            math_operation(commands, &pointer, DIV);
        }

        else if(strcmp(command, "out") == 0)
        {
            // out(commands, &pointer);
            commands[pointer] = OUT;
            pointer++;
        }

        else if(strcmp(command, "hlt") == 0)
        {
            commands[pointer] = HLT;
            pointer++;

        }
        else if(strcmp(command, "push_r") == 0)
        {
            commands[pointer] = PUSH_R;
            char* reg_type = (char* ) calloc(3, sizeof(char));
            sscanf(commands_buffer, "%s %n", reg_type, &pointer_scanf);
            commands_buffer = commands_buffer + pointer_scanf;
            
            if(strcmp(reg_type, "rax") == 0)
            {
                reg_type = command;
                commands[pointer + 1] = RAX;
                pointer += 2;
            }
            else if(strcmp(reg_type, "rbx") == 0)
            {
                reg_type = command;
                commands[pointer + 1] = RBX;
                pointer += 2;
            }
        }

        else if(strcmp(command, "pop_r") == 0)
        {
            commands[pointer] = POP_R;
            char reg_type[3] = {};
            sscanf(commands_buffer, "%3s %n", reg_type, &pointer_scanf);
            commands_buffer = commands_buffer + pointer_scanf;

            if(strcmp(reg_type, "rax") == 0)
            {
                commands[pointer + 1] = RAX;
                pointer += 2;
            }
            else if(strcmp(reg_type, "rbx") == 0)
            {
                commands[pointer + 1] = RBX;
                pointer += 2;
            }
        }

        else if(strcmp(command, "jmp") == 0)
        {
            commands[pointer] = JMP;
            int metka = 0;
            sscanf(commands_buffer, "%d: %n", &metka, &pointer_scanf);
            commands[pointer + 1] = mtk->labels[metka];
            commands_buffer = commands_buffer + pointer_scanf;

            pointer += 2;
        }

        else if (strcmp(command, "call") == 0)
        {
            commands[pointer] = CALL;
            int func = 0;
            sscanf(commands_buffer, "%d %n", &func, &pointer_scanf);
            commands[pointer + 1] = mtk->labels[func];
            commands_buffer = commands_buffer + pointer_scanf;

            pointer += 2;
        }

        else if (strcmp(command, "ret") == 0)
        {
            commands[pointer] = RET;
            
            pointer++;
        }

        counter++;
        number_of_operation++;

    }
    commands[0] = pointer - 1;

    return pointer;
}

void output_in_convert_commands(int* commands, int pointer, FILE* file_write)
{
    assert(commands);

    fwrite(commands, sizeof(int), pointer, file_write);

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
    int pointer_scanf = 0;
    commands[*pointer] = PUSH;
    // fprintf(stderr, "DAMIRRR");
    // fprintf(stderr, "commands[pointer] = %d  \n", commands[*pointer]);

    sscanf(*commands_buffer, "%d %n", &commands[*pointer + 1], &pointer_scanf);


    (*pointer) += 2;
    *commands_buffer = *commands_buffer + pointer_scanf;

    return 0;
}

int pop(int* commands, int* pointer, char** commands_buffer)
{
    int pointer_scanf = 0;
    commands[*pointer] = PUSH;

    sscanf(*commands_buffer, "%d %n", &commands[*pointer + 1], &pointer_scanf);


    (*pointer) += 2;
    *commands_buffer = *commands_buffer + pointer_scanf;

    return 0;
}

int math_operation(int* commands, int* pointer, enum commands operation)
{
    switch(operation)
    {
        case ADD:
        {
            commands[*pointer] = ADD;
            break;
        }
        case SUB:
        {
            commands[*pointer] = SUB;
            break;
        }
        case MUL:
        {
            commands[*pointer] = MUL;
            break;
        }
        case DIV:
        {
            commands[*pointer] = DIV;
            break;
        }
        default:
        {

            fprintf(stderr, "UNKWOWN OPERATION");
        }
    }
    
    *pointer++;

    return 0;
    
}

int out(int* commands, int* pointer)
{
    commands[*pointer] = OUT;
    *pointer++;

    return 0;
}