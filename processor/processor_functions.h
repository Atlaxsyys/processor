#pragma once

#include "processor.h"


int push_cpu(processor* CPU);
int math_operation_cpu(processor* CPU, int code_operation);
int out_cpu(processor* CPU);
int hlt_cpu(processor* CPU);
int jump_cpu(processor* CPU, int code_operation);
int get_index_reg(processor* CPU);
int call_cpu(processor* CPU);
int ret_cpu(processor* CPU);
int in_cpu(processor* CPU);
int push_reg_cpu(processor* CPU);
int pop_reg_cpu(processor* CPU, int deleted_number);
const char* converting_to_str(int proc_errors);
int binary_operation(processor* CPU, int (*operation)(int, int));
int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int divide(int a, int b);
int is_equal(int a, int b);
int is_not_equal(int a, int b);
int is_below(int a, int b);
int is_below_or_equal(int a, int b);
int is_above(int a, int b);
int is_above_or_equal(int a, int b);
int conditional_jump(processor* CPU, int (*cmp)(int, int));