#!/bin/bash
oc32-none-elf-gcc -nostdlib -fverbose-asm -fdump-rtl-all -Os -mzfsf -S ./test_cmov.c        -o ./test_cmov.S
oc32-none-elf-gcc -nostdlib -fverbose-asm -fdump-rtl-all -Os -mzfsf -S ./test_bitfield.c    -o ./test_bitfield.S
oc32-none-elf-gcc -nostdlib -fverbose-asm -fdump-rtl-all -Os -mzfsf -S ./test_postinc.c     -o ./test_postinc.S
