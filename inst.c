#include "inst.h"
#include "settings.h"

//arg1 = all_args >> 4
//arg2 = all_args & 0b00001111

//todo flags
//todo arg conv-s to func-s?

void inst_nop(uint8_t all_args, cpu_type* cpu) {}
void inst_set(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.reg[all_args>>4] = cpu->regs.reg[all_args&0b00001111];
}
void inst_vl0(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.v &= 0b1111111100000000;
    cpu->regs.v += all_args;
}
void inst_vl1(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.v &= 0b0000000011111111;
    cpu->regs.v += all_args << 8;
}
void inst_mmr(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.reg[all_args & 0b00001111] = cpu->mem[cpu->regs.reg[all_args>>4]];
}
void inst_mmw(uint8_t all_args, cpu_type* cpu) {
    cpu->mem[cpu->regs.reg[all_args>>4]] = cpu->regs.reg[all_args & 0b00001111];
}
void inst_str(uint8_t all_args, cpu_type* cpu) {
    (cpu->regs.s < size_mem-1) ? cpu->regs.s++ : 0;
    cpu->regs.reg[all_args&0b00001111] = cpu->mem[cpu->regs.s];
    cpu->mem[cpu->regs.s] = 0;
}
void inst_stw(uint8_t all_args, cpu_type* cpu) {
    cpu->mem[cpu->regs.s] = cpu->regs.reg[all_args&0b00001111];
    (cpu->regs.s > 0) ? cpu->regs.s-- : 0;
}
void inst_inc(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.reg[all_args&0b00001111]++;
}
void inst_dec(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.reg[all_args&0b00001111]--;
}
void inst_add(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.m = cpu->regs.reg[all_args>>4] + cpu->regs.reg[all_args&0b00001111];
}
void inst_sub(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.m = cpu->regs.reg[all_args>>4] - cpu->regs.reg[all_args&0b00001111];
}
void inst_mul(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.m = cpu->regs.reg[all_args>>4] * cpu->regs.reg[all_args&0b00001111];
}
void inst_div(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.m = cpu->regs.reg[all_args>>4] / cpu->regs.reg[all_args&0b00001111];
}
void inst_cmp(uint8_t all_args, cpu_type* cpu) {
    cpu->regs.f = 0;
    uint16_t val1 = cpu->regs.reg[all_args>>4];
    uint16_t val2 = cpu->regs.reg[all_args&0b00001111];
    if (val1 == val2) { cpu->regs.f |= (1 << 0); }
    if (val1 != val2) { cpu->regs.f |= (1 << 1); }
    if (val1 > val2) { cpu->regs.f |= (1 << 2); }
    if (val1 >= val2) { cpu->regs.f |= (1 << 3); }
    if (val1 < val2) { cpu->regs.f |= (1 << 4); }
    if (val1 <= val2) { cpu->regs.f |= (1 << 5); }
    // printf("f: %d", cpu->regs.f);
}
void inst_spi(uint8_t all_args, cpu_type* cpu) {
    if ((cpu->regs.f & (1 << (all_args&0b00001111))) >> (all_args&0b00001111)) { cpu->regs.p = cpu->regs.reg[all_args>>4]; }
}

const instruction instructions[] = {
    {"NOP", 0, {}, inst_nop},
    {"SET", 2, {REG, REG}, inst_set},
    {"VL0", 1, {VAL8}, inst_vl0},
    {"VL1", 1, {VAL8}, inst_vl1},
    {"MMR", 2, {REG, REG}, inst_mmr},
    {"MMW", 2, {REG, REG}, inst_mmw},
    {"STR", 1, {REG}, inst_str},
    {"STW", 1, {REG}, inst_stw},
    {"INC", 1, {REG}, inst_inc},
    {"DEC", 1, {REG}, inst_dec},
    {"ADD", 2, {REG, REG}, inst_add},
    {"SUB", 2, {REG, REG}, inst_sub},
    {"MUL", 2, {REG, REG}, inst_mul},
    {"DIV", 2, {REG, REG}, inst_div},
    {"CMP", 2, {REG, REG}, inst_cmp},
    {"SPI", 2, {REG, VAL4}, inst_spi},
};
const size_t instructions_count = sizeof(instructions)/sizeof(instruction);

// void (*const instructions[])(uint8_t, cpu_type*) = { inst_nop, inst_set };
