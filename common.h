#pragma once

#include <stdio.h>

#include "stack.h"

enum commands
{
    HLT = -1,
    OUT = 0,
    PUSH = 1,
    POP = 2,
    ADD = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    SQRT = 7,
    IN = 8,
    PUSH_R = 11,
    POP_R = 12,
    JMP = 13,
    JE = 14,
    JNE = 15,
    JB = 16,
    RAX = 20,
    RBX = 21,
    RCX = 22,
    RDX = 23,
    CALL = 30,
    RET = 31
};
