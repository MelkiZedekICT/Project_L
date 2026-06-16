; -------------------------------------------------------------------
; isr_stub.asm – NASM assembly stubs for the first 32 CPU exceptions
; -------------------------------------------------------------------

bits 32

; Export the ISR entry points so C++ can see them
global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
global isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
global isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
global isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

; Import the C++ fault handler
extern fault_handler

; -------------------------------------------------------------------
; Macro to generate an ISR stub.
;   %1 = interrupt number
;   %2 = 1 if the CPU pushes an error code, else 0
; -------------------------------------------------------------------
%macro ISR_STUB 2
    isr%1:
        ; If the CPU does not supply an error code, push a dummy one
        %if %2 == 0
            push dword 0
        %endif

        ; Save all general‑purpose registers
        pusha

        ; Push the error code (the one the CPU pushed or our dummy)
        ; After pusha, the original error code is at [esp+32]
        push dword [esp + 32]

        ; Push the interrupt number (first argument to fault_handler)
        push dword %1

        ; Call the C++ handler
        call fault_handler

        ; Clean up the two arguments (8 bytes)
        add esp, 8

        ; Restore registers
        popa

        ; Remove the error code (dummy or real) from the stack
        add esp, 4

        ; Return from interrupt
        iret
%endmacro

; -------------------------------------------------------------------
; Generate stubs for exceptions 0–31.
; Error‑code list: 8, 10, 11, 12, 13, 14, 17 have error codes.
; -------------------------------------------------------------------
ISR_STUB 0,  0
ISR_STUB 1,  0
ISR_STUB 2,  0
ISR_STUB 3,  0
ISR_STUB 4,  0
ISR_STUB 5,  0
ISR_STUB 6,  0
ISR_STUB 7,  0
ISR_STUB 8,  1      ; double fault
ISR_STUB 9,  0
ISR_STUB 10, 1      ; invalid TSS
ISR_STUB 11, 1      ; segment not present
ISR_STUB 12, 1      ; stack segment
ISR_STUB 13, 1      ; general protection
ISR_STUB 14, 1      ; page fault
ISR_STUB 15, 0
ISR_STUB 16, 0
ISR_STUB 17, 1      ; alignment check
ISR_STUB 18, 0
ISR_STUB 19, 0
ISR_STUB 20, 0
ISR_STUB 21, 0
ISR_STUB 22, 0
ISR_STUB 23, 0
ISR_STUB 24, 0
ISR_STUB 25, 0
ISR_STUB 26, 0
ISR_STUB 27, 0
ISR_STUB 28, 0
ISR_STUB 29, 0
ISR_STUB 30, 0
ISR_STUB 31, 0

global irq1
extern keyboard_handler_main

irq1:
    pusha                   ; Save all CPU registers
    call keyboard_handler_main ; Jump to our C++ driver
    popa                    ; Restore registers
    iret                    ; Return from interrupt
