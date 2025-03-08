#pragma once

#include <stdio.h>

void output_in_convert_commands(int* commands, int pointer, FILE* file_write_second);
long size_commands(FILE* file_read);
char* create_buffer(FILE* file_read);
int n_string(char* commands_buffer, long size_command);