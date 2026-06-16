#include "io.h"
#include <stdint.h>

extern "C" void keyboard_handler_main();

// Standard US QWERTY Scancode Translation Table (Lowercase only for now)
const char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Global cursor tracking
int cursor_x = 0;
int cursor_y = 3; // Start typing below our OS header

void print_char(char c) {
    volatile char* video_memory = (volatile char*)0xB8000;
    
    if (c == '\b') { // Handle Backspace
        if (cursor_x > 0) {
            cursor_x--;
            video_memory[(cursor_y * 80 + cursor_x) * 2] = ' ';
        }
    } else if (c == '\n') { // Handle Enter Key
        cursor_x = 0;
        cursor_y++;
    } else { // Handle Normal Characters
        video_memory[(cursor_y * 80 + cursor_x) * 2] = c;
        video_memory[(cursor_y * 80 + cursor_x) * 2 + 1] = 0x0A; // Green text
        cursor_x++;
    }

    // Wrap around to the next line if we hit the edge of the screen
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
}

extern "C" void keyboard_handler_main() {
    // Port 0x64 is the status port. Port 0x60 is the data port.
    uint8_t status = inb(0x64);
    
    if (status & 0x01) {
        uint8_t scancode = inb(0x60);
        
        // If the highest bit is 0, it's a key press (not a release)
        if (!(scancode & 0x80)) {
            char c = kbd_us[scancode];
            if (c != 0) {
                print_char(c);
            }
        }
    }
    
    // Send End of Interrupt (EOI) to the PIC so it allows the next keypress
    outb(0x20, 0x20);
}
