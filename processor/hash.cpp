#include "stack.h"

hash_elem hash_djb2 (const char* str, size_t size_of_str)
{
    hash_elem hash = 5381;

    for (size_t i = 0; i < size_of_str; i++)
    {
        hash_elem a = (hash_elem) *str++;
        hash = ((hash << 5) + hash) + a;
    }

    return hash;
}