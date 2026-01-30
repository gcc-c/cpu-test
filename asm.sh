#!/bin/bash
gcc -Wno-unused-parameter -Wall -Wextra asm.c f.c inst.c -o asm && ./asm $@
