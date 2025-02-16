
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int assembler(int* commands, FILE* file_read);
void printf_in_convert_commands(int* commands, int pointer);

int main()
{
    int commands[1000] = {};

    FILE* file_read = fopen("comands.txt", "r");

    int pointer = assembler(commands, file_read);

    fclose(file_read);

    printf_in_convert_commands(commands, pointer);
}



int assembler(int* commands, FILE* file_read)
{

    char command[6];
    int pointer = 1;
    int number_of_operation = 0;

    while(!feof(file_read))
    {
        fscanf(file_read, "%s", command);

        
        if(strcmp(command, "push") == 0)
        {
            commands[pointer] = 1;

            fscanf(file_read, "%d", &commands[pointer + 1]);

            pointer += 2;
            number_of_operation++;
        }

        else if(strcmp(command, "pop") == 0)
        {
            commands[pointer] = 2;

            fscanf(file_read, "%d", &commands[pointer + 1]);

            pointer += 2;
            number_of_operation++;
        }

        else if(strcmp(command, "add") == 0)
        {
            commands[pointer] = 3;
            number_of_operation++;
            pointer++;
        }

        else if(strcmp(command, "sub") == 0)
        {
            commands[pointer] = 4;
            number_of_operation++;
            pointer++;
        }

        else if(strcmp(command, "out") == 0)
        {
            commands[pointer] = 0;
            number_of_operation++;
            pointer++;
        }

        else if(strcmp(command, "hlt") == 0)
        {
            commands[pointer] = -1;
            number_of_operation++;
            pointer++;

        }

    }
    commands[0] = pointer - 1;

    return pointer;
}

void printf_in_convert_commands(int* commands, int pointer)
{
    FILE* file_writing = fopen("converted_commands.bin", "wb");

    fwrite(commands, sizeof(int), pointer, file_writing);

    fclose(file_writing);
}