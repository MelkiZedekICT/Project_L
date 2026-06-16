#!/bin/bash
set -e # THIS IS CRITICAL: It forces the script to stop if the compiler fails.

echo "Assembling Bootloader and ISR Stubs..."
nasm -f elf32 boot.asm -o boot.o
nasm -f elf32 isr_stub.asm -o isr_stub.o

echo "Compiling C++ Kernel and Subsystems..."
g++ -m32 -c kernel.cpp kstring.cpp idt.cpp keyboard.cpp -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

echo "Linking into an Operating System Binary..."
ld -m elf_i386 --no-warn-rwx-segments -T linker.ld -o myos.bin boot.o isr_stub.o kernel.o kstring.o idt.o keyboard.o

echo "Launching Emulation (Close the QEMU window to stop)..."
qemu-system-i386 -kernel myos.bin
