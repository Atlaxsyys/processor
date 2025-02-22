#pragma once

#include <stdio.h>

const int SIZE_COMMANDS = 200;
const int SIZE_LBL = 100;


struct lbl
{
    char labels[SIZE_LBL];
    int addr;
};


int assembler(int* commands, FILE* file_read, lbl* mtk);
void printf_in_convert_commands(int* commands, int pointer, FILE* file_write_second);