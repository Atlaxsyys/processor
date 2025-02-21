#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>

typedef int stackElem;
typedef unsigned long hash_elem;

struct stack
{

    stackElem* data;
    int size;
    int capacity;

    hash_elem hash;

};



enum errors
{
    STK_IS_NULL = 1 << 0,

    STACK_DATA_IS_NULL = 1 << 1,

    STACK_CAPACITY_LESS_SIZE = 1 << 2,

    STACK_SIZE_IS_ERROR = 1 << 3,

    STACK_CAPACITY_IS_ERROR = 1 << 4,

    CANARY_START_CHANGE = 1 << 5,

    CANARY_END_CHANGE = 1 << 6
};

int stack_constructor(struct stack* stk, int capasity);
int stack_destroy(struct stack* stk);
int stack_dump(struct stack* stk);
int stack_error(struct stack* stk);
int error_output(int errors);
int stack_assert(struct stack* stk);
void stack_realloc(struct stack* stk);
int stack_push (struct stack* stk, stackElem number);
int stack_pop(struct stack* stk);
hash_elem hash_djb2 (const char* str, size_t size_of_str);
void convert_to_binary(unsigned char number);

#endif // STACK_H_ 