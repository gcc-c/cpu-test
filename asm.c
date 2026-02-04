#include "f.h"
#include "settings.h"
#include "inst.h"
// #include <stdint.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*typedef struct {
    bool val;
    bool reg;
} arg;*/

/*enum arg {VAL, REG};

typedef struct {
    char* op;
    // uint8_t id;
    char args_count;
    // arg args[2];
    enum arg args[2];
} instruction;*/

/*instruction instructions[] = {
    {"NOP", 0, {}},
    {"SET", 2, {REG, REG}},
    {"VL0", 1, {VAL}},
    {"VL1", 1, {VAL}},
    {"MMR", 2, {REG, REG}},
    {"MMW", 2, {REG, REG}},
    {"STR", 1, {REG}},
    {"STW", 1, {REG}},
    {"INC", 1, {REG}},
    {"DEC", 1, {REG}},
    {"ADD", 2, {REG, REG}},
    {"SUB", 2, {REG, REG}},
    {"MUL", 2, {REG, REG}},
    {"DIV", 2, {REG, REG}},
};*/

//todo
char regs[] = {'P', 'S', 'F', 'V', 'M', ' ', ' ', ' ', ' ', ' ', 'A', 'B', 'C', 'X', 'Y', 'Z'};

int parse_val(char* str, int warning_limit) {
    //todo bases
    //todo test ts
    long r = strtol(str, 0, 10);
    // printf("l %ld\n", r);
    if (labs(r) >= warning_limit) { printf("warning: val doesnt fit\n"); }
    return r;
}

int parse_reg(char* str) {
    if (str[1] != '\0') { return strtol(str, 0, 10); }
    for (size_t i = 0; i < sizeof(regs)/sizeof(char); i++) {
        if (regs[i] == toupper(str[0])) { return i; }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) { error("no prog file provided!"); }

    char* bin_name;

    if (argc > 2) { bin_name = argv[2]; }
    else {
        bin_name = malloc(strlen(argv[1])+sizeof(".bin"));
        sprintf(bin_name, "%s.bin", argv[1]);
    }

    FILE* source = fopen(argv[1], "r");
    if (!source) { error("file doesnt exist!"); }
    
    FILE* bin = fopen(bin_name, "wb");
    if (!bin) { error("err opening bin output file"); }
    
    char buf[256];

    int line = 0;
    while (fgets(buf, sizeof(buf)/sizeof(char), source)) {
        line++; //с 1
        int n = 0;
        int l = 0;
        union {
            struct {
                char op[256];
                char arg1[256];
                char arg2[256];
            };
            char i[3][256];
        } inst = {};
        // memset(inst.i, 0, 256*3*sizeof(char));
        // printf("op: %s\narg1: %s\narg2: %s\n", inst.op, inst.arg1, inst.arg2);
        // return 1;
        // char op[256] = "";
        // char arg1[256] = "";
        // char arg2[256] = "";
        if (buf[0] == '#') {
            uint16_t raw = parse_val(buf+1, size_mem);
            uint8_t raw1 = raw >> 8;
            uint8_t raw2 = raw & 0b0000000011111111;
            fwrite(&raw1, sizeof(raw1), 1, bin);
            fwrite(&raw2, sizeof(raw2), 1, bin);
            // fwrite(&raw, sizeof(raw), 1, bin);
            continue;
        }
        for (size_t i = 0; i < sizeof(buf)/sizeof(char); i++) {
            if (buf[i] == '\n' || buf[i] == '\0' || (buf[i] == '/' && i < sizeof(buf)/sizeof(char)-1 && buf[i+1] == '/')) {
                break;
            }
            if (buf[i] == ' ') {
                n++;
                l = 0;
                continue;
            }
            if (n > 2) { error("too many args!"); }
            inst.i[n][l] = toupper(buf[i]);
            /*switch (n) {
                case 0:
                    op[l] = buf[i];
                    break;
            }*/
            l++;
            // if (n == 0) {
                // op += buf[i];
            // }
            // printf("%c\n", buf[i]);
        }
        // printf("op: %s\narg1: %s\narg2: %s\n", inst.op, inst.arg1, inst.arg2);
        // printf("%s\n", op);
        if (n == 0 && l == 0) { continue; }

        uint8_t op;
        uint8_t all_args = 0;

        bool g = 0;
        for (size_t i = 0; i < instructions_count; i++) {
            // printf("%s %s\n", inst.op, instructions[i].op);
            if (strcmp(inst.op, instructions[i].op) == 0) {
                g = 1;
                op = i;
                // printf("%zu\n", i);
                // printf("%d\n", inst.arg2[0] == '\0');
                int args_count = (inst.arg1[0] != '\0') + (inst.arg2[0] != '\0');
                // if ((inst.arg2[0] == '\0' && instructions[i].args_count >= 2) || (inst.arg1[0] == '\0' && instructions[i].args_count >= 1)) {
                if (args_count != instructions[i].args_count) {
                    printf("line %d\n", line);
                    // error("args dont match");
                }
                if (args_count > instructions[i].args_count) { error("too many args"); }
                else if (args_count < instructions[i].args_count) { error("not enough args"); }
                
                if (instructions[i].args_count == 0) { break; }
                // if (instructions[i].args[0] == VAL) { all_args = parse_val(inst.arg1, size_mem); break; }
                // if (instructions[i].args_count == 1) { all_args = parse_reg(inst.arg1); break; }
                // all_args = parse_reg(inst.arg1) << 4;
                // all_args += parse_reg(inst.arg2); break;
                if (instructions[i].args_count == 1) {
                    // all_args = (instructions[i].args[0] == REG) ? parse_reg(inst.arg1) : parse_val(inst.arg1, size_mem); break;
                    all_args = (instructions[i].args[0] == REG) ? parse_reg(inst.arg1) : ((instructions[i].args[0] == VAL8) ? parse_val(inst.arg1, size_val8) : parse_val(inst.arg1, size_reg));
                    break;
                }
                all_args = ((instructions[i].args[0] == REG) ? parse_reg(inst.arg1) : parse_val(inst.arg1, size_reg)) << 4;
                all_args += (instructions[i].args[1] == REG) ? parse_reg(inst.arg2) : parse_val(inst.arg2, size_reg);
            }
        }
        if (!g) { printf("line %d\n", line); error("unknown op"); }
        // printf("line %d\nop: %d\nargs: %d\n", line, op, all_args);
        fwrite(&op, sizeof(op), 1, bin);
        fwrite(&all_args, sizeof(all_args), 1, bin);
    }

    fclose(source);
    fclose(bin);

    return 0;
}
