#pragma once

#include <stdio.h>


enum commands
{
    HLT     = -1,
    OUT     = 10,
    PUSH    = 11,
    POP     = 12,
    ADD     = 13,
    SUB     = 14,
    MUL     = 15,
    DIV     = 16,
    SQRT    = 17,
    IN      = 18,
    PUSH_R  = 21,
    POP_R   = 22,
    JMP     = 23,
    JE      = 24,
    JNE     = 25,
    JB      = 26,
    JBE     = 27,
    JA      = 28,
    JAE     = 29,
    CALL    = 30,
    RET     = 31
};

enum registrs
{
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
};
