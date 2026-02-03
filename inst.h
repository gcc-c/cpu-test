#pragma once
#include "settings.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef union {
    struct {
        uint16_t p,s,f,v,m;
    };
    uint16_t reg[size_reg];
} registers;

typedef struct {
    uint16_t mem[size_mem];
    registers regs;
} cpu_type;

enum arg {VAL4, VAL8, REG};

typedef struct {
    char* op;
    // uint8_t id;
    char args_count;
    // arg args[2];
    enum arg args[2];
    void (*func)(uint8_t all_args, cpu_type* cpu);
} instruction;

extern const instruction instructions[];
extern const size_t instructions_count;

// extern void (*const instructions[])(uint8_t, cpu_type*);
