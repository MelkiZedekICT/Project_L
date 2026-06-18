#include <stdint.h>
#include <stddef.h>
#include "idt.h"
#include "io.h"

extern "C" void kernel_main();
extern "C" void irq1();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void clear_screen(); // Pointing to the one in keyboard.cpp!

int cursor_x = 0;
int cursor_y = 2;

void print_boot_string(const char* message, int row) {
    volatile char* video_memory = (volatile char*)0xB8000;
    int j = (row * 80) * 2;
    for (int i = 0; message[i] != '\0'; ++i) {
        video_memory[j] = message[i];
        video_memory[j+1] = 0x0A; // Green text
        j += 2;
    }
}

void remap_pic() {
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28); 
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    
    // Silence the timer
    outb(0x21, 0xFD); outb(0xA1, 0xFF);  
}

void kernel_main() {
   
    clear_screen();
    
  
    print_boot_string("Project L: Ring 0 Interactive Shell Initialized.", 0);
    print_boot_string("Terminal ready. Type 'help' to see commands:", 1);
    
   
    cursor_x = 0;
    cursor_y = 2;
    
    // Initialize hardware routing
    init_idt();
    remap_pic();
    
    // Map the keyboard
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    
    // Enable hardware interrupts
    asm volatile("sti");
    
    // Halt and wait for user input
    while(1) {
        asm volatile("hlt"); 
    }
}
