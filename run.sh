#!/bin/bash
gcc -Wno-unused-parameter -Wall -Wextra main.c f.c inst.c -o main && ./main $@
