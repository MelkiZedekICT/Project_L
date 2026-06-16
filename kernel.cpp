#include <stdint.h>
#include <stddef.h>
#include "idt.h"
#include "io.h"

extern "C" void kernel_main();
extern "C" void irq1();

// Let the compiler know this function exists in idt.cpp
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

void clear_screen() {
    volatile char* video_memory = (volatile char*)0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x0A; // Black background, Green text
    }
}

void print_string(const char* message, int row) {
    volatile char* video_memory = (volatile char*)0xB8000;
    int j = (row * 80) * 2;
    for (int i = 0; message[i] != '\0'; ++i) {
        video_memory[j] = message[i];
        video_memory[j+1] = 0x0A;
        j += 2;
    }
}

void remap_pic() {
    // Move hardware interrupts to start at 32 (avoids CPU exception overlap)
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28); 
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    
    // Silence the hardware timer (0xFD) and the secondary PIC (0xFF).
    // This stops the unhandled interrupts that were crashing the screen.
    outb(0x21, 0xFD); outb(0xA1, 0xFF);  
}

void kernel_main() {
    clear_screen();
    print_string("Project L: General OS Mode Initialized.", 0);
    print_string("Terminal ready. Type your commands below:", 1);
    
    // Initialize security gates and hardware routing
    init_idt();
    remap_pic();
    
    // Map the keyboard interrupt (IRQ1 -> Gate 33) to our assembly stub
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    
    // Enable hardware interrupts
    asm volatile("sti");
    
    // Halt the CPU until a hardware interrupt (like a keystroke) occurs
    while(1) {
        asm volatile("hlt"); 
    }
}
