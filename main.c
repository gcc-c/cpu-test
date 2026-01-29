#include "settings.h"
#include "inst.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef union {
    struct {
        uint16_t p,s,f,v;
    };
    uint16_t reg[size_reg];
} registers;

struct {
    uint16_t mem[size_mem];
    registers regs;
} cpu;

void error(char* name) {
    printf("err: %s\n", name);
    exit(1);
}

void prepare(char* name) {
    cpu.regs.s = size_mem-1;

    FILE* file = fopen(name, "rb");
    for (uint16_t i = 0; !feof(file); i++) {
        uint16_t buf;
        fread(&buf, sizeof(buf), 1, file);
        cpu.mem[i] = buf;
    }
}

void print_num(int n) {
    //todo bin?
    // printf("%5d | %#4x", n, n);
    printf("%5d | 0x%04x", n, n);
}

void print_mem_line(int i) {
    print_num(i);
    printf(": ");
    print_num(cpu.mem[i]);
    printf("\n");
}

void print_memory() {
    printf("cpu.mem:\n");
    int empty = 0;
    for (int i = 0; i < size_mem; i++) {
        if (cpu.mem[i] == 0 && (empty > 0 || (i < size_mem-1 && cpu.mem[i+1] == 0))) { empty++; }
        else {
            if (empty) { printf(".....\n"); empty = 0; }
            print_mem_line(i);
        }
    }
    if (empty) { printf(".....\n"); }
    /*int empty_lines = 0;
    for (int i = 0; i < size_mem; i++) {
        if (cpu.mem[i] == 0 && (i == size_mem-1 || cpu.mem[i+1] == 0)) { empty_lines++; }
        // if (cpu.mem[i] != 0 || i == size_mem-1) {
        else {
            // if (empty_lines > 1) { printf("..... | 0x.... 0*%d\n", empty_lines); }
            if (empty_lines > 1) { printf("..... | 0x....: "); print_num(0); printf("\n"); }
            print_num(i);
            // printf(": %d\n", cpu.mem[i]);
            printf(": ");
            print_num(cpu.mem[i]);
            printf("\n");
        }
    }*/
}

void print_regs() {
    printf("cpu.regs:\n");
    // char reg_names[] = {'p','s','f','v',};
    for (int i = 0; i < size_reg; i++) {
        printf("%02d: %d\n", i, cpu.regs.reg[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) { error("no prog bin file provided!"); }

    prepare(argv[1]);

    // print_memory();
    // print_regs();

    return 0;
}
