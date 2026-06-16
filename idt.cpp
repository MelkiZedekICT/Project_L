#include "idt.h"

struct idt_entry_t {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

idt_entry_t idt[256];
idt_ptr_t idtp;

extern "C" void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

extern "C" void init_idt() {
    idtp.limit = (sizeof(idt_entry_t) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    uint8_t* p = (uint8_t*)&idt;
    for(int i = 0; i < sizeof(idt_entry_t) * 256; i++) p[i] = 0;

    asm volatile("lidt %0" : : "m" (idtp));
}

extern "C" void fault_handler(uint32_t int_no, uint32_t err_code) {
    (void)int_no; (void)err_code; // Suppress unused variable warnings
    volatile char* video_memory = (volatile char*)0xB8000;
    const char* msg = "CPU EXCEPTION CAUGHT";
    for(int i=0; msg[i] != '\0'; i++) {
        video_memory[i*2] = msg[i];
        video_memory[i*2+1] = 0x04; // Red text for real errors
    }
    while(1) asm volatile("hlt");
}
