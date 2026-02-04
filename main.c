#include "settings.h"
#include "inst.h"
#include "f.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*typedef union {
    struct {
        uint16_t p,s,f,v;
    };
    uint16_t reg[size_reg];
} registers;

struct {
    uint16_t mem[size_mem];
    registers regs;
} cpu;*/

cpu_type cpu;

void prepare(char* name) {
    cpu.regs.s = size_mem-1;

    FILE* file = fopen(name, "rb");
    
    if (!file) { error("file doesnt exist!"); }

    // uint16_t buf;
    uint8_t op, all_args;
    uint16_t i = 0;
    // for (uint16_t i = 0; !feof(file); i++) {
    // while (fread(&buf, sizeof(buf), 1, file)) {
    while (fread(&op, sizeof(op), 1, file)) {
        // printf("%d\n", buf);
        fread(&all_args, sizeof(all_args), 1, file);
        // cpu.mem[i] = buf;
        cpu.mem[i] = (op << 8) + all_args;
        i++;
    }
    
    fclose(file);
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

void execute(uint16_t inst) {
    uint8_t op = inst >> 8;
    uint8_t all_args = inst;
    // printf("%d: %d; %d\n", inst, op, all_args); error("123");
    instructions[op].func(all_args, &cpu);
}

bool auto_execution = 1;
bool debug = 0;

int run() {
    while (1) {
        // printf("%d\n", cpu.mem[4]);
        execute(cpu.mem[cpu.regs.p]);
        cpu.regs.p++;
        // printf("p: %d\n", cpu.regs.p);
        // system("sleep 1");
        // sleep(1);
        // getchar();
        if (!debug) { continue; }
        print_memory();
        print_regs();
        if (auto_execution) { continue; }
        switch (getchar()) {
            case key_auto:
                auto_execution = 1; break;
            case key_exit:
                return 1; break;
            default:
                continue;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) { error("no prog bin file provided!"); }

    if (argc > 2) {
        if (strcmp(argv[2], "-d") == 0) {
            debug = 1;
            auto_execution = 0;
        }
    }
    
    prepare(argv[1]);

    // print_memory();
    // print_regs();

    return run();
}
