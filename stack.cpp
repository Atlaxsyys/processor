#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "stack.h"
#include "colour.h"

const stackElem CANARY_START = 0xBAD;
const stackElem CANARY_END = 0xDED;

const stackElem POIZON_NUMBER = 0;



int stack_constructor(struct stack* stk, int capacity)
{
    assert(stk);

    stk->data = (stackElem*) calloc((size_t) capacity + 2, sizeof(stackElem));
    stk->capacity = capacity;
    stk->size = 0;


    stk->data[0] = CANARY_START;
    stk->data[capacity + 1] = CANARY_END;

    for (int i = 1; i <= capacity; i++)
    {
        stk->data[stk->size + i] = POIZON_NUMBER;
    }

    stk->hash = hash_djb2((const char*) stk, sizeof(*stk));

    stack_assert(stk);

    return 0;
}

int stack_destroy(struct stack* stk)
{
    stack_assert(stk);

    free(stk->data);
    stk->size = -1;
    stk->capacity = -1;
    stk->data = NULL;

    return 0;
}

int stack_dump(struct stack* stk)
{
    fprintf(stderr, YELLOW "______________________________________________________\n" CLEAR);
    fprintf(stderr, GREEN "CANARY_START = %d\n" CLEAR, stk->data[0]);
    for (int i = 1; i <= stk->capacity; i++)
        printf (BLUE "stack->data[%d] = %d\n" CLEAR, i, stk->data[i]);

    fprintf(stderr, GREEN "CANARY_END = %d\n" CLEAR, stk->data[stk->capacity + 1]);

    fprintf(stderr, RED "\n\nstack->data adress: %p\n" CLEAR, stk->data);
    fprintf(stderr, PURPLE "stack->capacity:    %d\n" CLEAR, stk->capacity);
    fprintf(stderr, PURPLE"stack->size:        %d\n" CLEAR, stk->size);
    fprintf(stderr, RED "hash:               %lu\n" CLEAR,  stk->hash);
    fprintf(stderr, YELLOW "______________________________________________________\n\n" CLEAR);

    getchar();

    return 0;
}

int stack_assert(struct stack* stk)
{
    int errors = stack_error(stk);


    if (errors != 0)
    {
        error_output(errors);
        abort();
    }

    return 0;
}

int stack_error(struct stack* stk)
{
    int errors = 0;

    if(stk == NULL)
    {
        errors |= STK_IS_NULL;
    }
    if (stk->data == NULL)
    {
        errors |= STACK_DATA_IS_NULL;
    }
    if(stk->size > stk->capacity)
    {
        errors |= STACK_CAPACITY_LESS_SIZE;
    }
    if (stk->size < 0)
    {
        errors |= STACK_SIZE_IS_ERROR;
    }
    if (stk->capacity < 0)
    {
        errors |= STACK_CAPACITY_IS_ERROR;
    }
    if (stk->data != NULL && stk->data[0] != CANARY_START)
    {
        errors |= CANARY_START_CHANGE;
    }
    if (stk->data != NULL && stk->data[stk->capacity + 1] != CANARY_END)
    {
        errors |= CANARY_END_CHANGE;
    }

    return errors;
}

int error_output(int errors)
{
    fprintf(stderr, RED "Error: " CLEAR);

    convert_to_binary((unsigned char) errors);

    switch(errors)
    {
        case STK_IS_NULL:
            fprintf(stderr, RED " - STK_IS_NULL\n" CLEAR);
            break;
        case STACK_DATA_IS_NULL:
            fprintf(stderr, RED " - STACK_DATA_IS_NULL\n" CLEAR);
            break;
        case STACK_CAPACITY_LESS_SIZE:
            fprintf(stderr, RED " - STACK_CAPACITY_LESS_SIZE\n" CLEAR);
            break;
        case STACK_SIZE_IS_ERROR:
            fprintf(stderr, RED " - STACK_SIZE_IS_ERROR\n" CLEAR);
            break;
        case STACK_CAPACITY_IS_ERROR:
            fprintf(stderr, RED " - STACK_CAPACITY_IS_ERROR\n" CLEAR);
            break;
        case CANARY_START_CHANGE:
            fprintf(stderr, RED " - CANARY_START_CHANGE\n" CLEAR);
            break;
        case CANARY_END_CHANGE:
            fprintf(stderr, RED " - CANARY_END_CHANGE\n" CLEAR);
            break;
        default:
            fprintf(stderr, RED " - UNKNOWN ERROR\n" CLEAR);
        
    }

    return 0;
}

void stack_realloc(struct stack* stk)
{
    const int scale_factor = 2;
    
    stk->capacity *= scale_factor;
    
    stk->data = (stackElem*) realloc (stk->data, stk->capacity * sizeof (stk->data[0]) + 2* sizeof(stk->data[0]));

    for (int i = stk->size + 1; i <= stk->capacity; i++)
    {
        stk->data[i] = POIZON_NUMBER;
    }
    stk->data[stk->capacity + 1] = CANARY_END; 

}

int stack_push(struct stack* stk, stackElem number)
{
    stack_assert(stk);
    if (stk->capacity <= stk->size)
    {
        stack_realloc(stk);
    }

    stk->data[stk->size + 1] = number;
    stk->size++;

    stk->hash = hash_djb2((const char*) stk, sizeof(*stk));

    stack_assert(stk);
    
    return 0;
}

int stack_pop(struct stack* stk)
{
    stack_assert(stk);
    
    stk->size--;
    stackElem deleted_value = stk->data[stk->size + 1];
    stk->data[stk->size + 1] = POIZON_NUMBER;

    stk->hash = hash_djb2((const char*) stk, sizeof(*stk));

    stack_assert(stk);

    return deleted_value;
}

void convert_to_binary(unsigned char number)
{
    const int BITS = 8;
    char binary[BITS + 1];
    
    binary[BITS] = '\0';

    for (int i = BITS - 1; i >= 0; i--)
    {
        binary[i] = (number & 1) ? '1' : '0';
        number >>= 1;
    }

    printf("%s\n", binary);
}

