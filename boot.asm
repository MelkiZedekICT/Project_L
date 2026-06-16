; Multiboot 1 Header
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB for the kernel stack
stack_top:

section .text
global _start
extern kernel_main

_start:
    ; Set up the stack pointer
    mov esp, stack_top

    ; Call our C++ kernel
    call kernel_main

    ; If the kernel ever exits, put the computer into an infinite loop
    cli
.hang:
    hlt
    jmp .hang
