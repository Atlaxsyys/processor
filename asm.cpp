#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "asm_proc.h"



int main(int argc, char* argv[])
{
    lbl mtk = {};

    int* commands = (int*)calloc(SIZE_COMMANDS, sizeof(int));

    FILE* file_read = fopen(argv[1], "r");
    if(!file_read)
    {
        fprintf(stderr, "Error open file");
        abort();
    }

    int pointer = assembler(commands, file_read, &mtk);
    rewind(file_read);
    assembler(commands, file_read, &mtk);


    fclose(file_read);

    FILE* file_write = fopen(argv[2], "wb");
    if(!file_write)
    {
        fprintf(stderr, "Error open file");
        abort();
    }

    printf_in_convert_commands(commands, pointer, file_write);

    fclose(file_write);
}



int assembler(int* commands, FILE* file_read, lbl* mtk)
{
    assert(commands);

    char command[6];
    int pointer = 1;
    int number_of_operation = 0;

    while(!feof(file_read))
    {
        fscanf(file_read, "%s", command);

        if(strchr(command, ':') != NULL)
        {
            int number_mtk = 0;
            sscanf(command, "%d:", &number_mtk);
            mtk->labels[number_mtk] = pointer - 1;  // because commands[0] = number of operations
        }

        
        if(strcmp(command, "push") == 0)
        {
            commands[pointer] = PUSH;

            fscanf(file_read, "%d", &commands[pointer + 1]);

            pointer += 2;
        }

        else if(strcmp(command, "pop") == 0)
        {
            commands[pointer] = POP;

            fscanf(file_read, "%d", &commands[pointer + 1]);

            pointer += 2;
        }

        else if(strcmp(command, "add") == 0)
        {
            commands[pointer] = ADD;
            pointer++;
        }

        else if(strcmp(command, "sub") == 0)
        {
            commands[pointer] = SUB;
            pointer++;
        }

        else if(strcmp(command, "out") == 0)
        {
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
            fscanf(file_read, "%s", command);
            char* reg_type = (char* ) calloc(3, sizeof(char));

            if(strcmp(command, "rax") == 0)
            {
                reg_type = command;
                commands[pointer + 1] = RAX;
                pointer += 2;
            }
            else if(strcmp(command, "rbx") == 0)
            {
                reg_type = command;
                commands[pointer + 1] = RBX;
                pointer += 2;
            }
            
            
        }

        else if(strcmp(command, "pop_r") == 0)
        {
            commands[pointer] = POP_R;
            char* reg_type = (char* ) calloc(3, sizeof(char));
            fscanf(file_read, "%s", reg_type);

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
            fscanf(file_read, "%d", &metka);
            commands[pointer + 1] = mtk->labels[metka];

            pointer += 2;
        }
        number_of_operation++;

    }
    commands[0] = pointer - 1;

    return pointer;
}

void printf_in_convert_commands(int* commands, int pointer, FILE* file_write)
{
    assert(commands);

    fwrite(commands, sizeof(int), pointer, file_write);

}
