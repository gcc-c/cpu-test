#pragma once
#include "settings.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

void inst_nop(uint8_t all_args, cpu_type* cpu);
void inst_set(uint8_t all_args, cpu_type* cpu);
void inst_vl0(uint8_t all_args, cpu_type* cpu);
void inst_vl1(uint8_t all_args, cpu_type* cpu);
void inst_mmr(uint8_t all_args, cpu_type* cpu);
void inst_mmw(uint8_t all_args, cpu_type* cpu);
void inst_str(uint8_t all_args, cpu_type* cpu);
void inst_stw(uint8_t all_args, cpu_type* cpu);
void inst_inc(uint8_t all_args, cpu_type* cpu);
void inst_dec(uint8_t all_args, cpu_type* cpu);
void inst_add(uint8_t all_args, cpu_type* cpu);
void inst_sub(uint8_t all_args, cpu_type* cpu);
void inst_mul(uint8_t all_args, cpu_type* cpu);
void inst_div(uint8_t all_args, cpu_type* cpu);
void inst_cmp(uint8_t all_args, cpu_type* cpu);
void inst_spi(uint8_t all_args, cpu_type* cpu);

extern const instruction instructions[];
extern const size_t instructions_count;

// extern void (*const instructions[])(uint8_t, cpu_type*);
