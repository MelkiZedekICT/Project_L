#include <stdint.h>
#include "io.h"

// --- The Global Input Buffer & Cursor ---
char command_buffer[256];
int buffer_index = 0;

extern int cursor_x;
extern int cursor_y;

// Standard US QWERTY Scancode Map (Lowercase only for now)
const char kbd_US[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,   
  '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*', 0, ' '
};

// --- Screen Interaction Helpers ---
void print_char(char c) {
    volatile char* video_memory = (volatile char*)0xB8000;
    int offset = (cursor_y * 80 + cursor_x) * 2;
    video_memory[offset] = c;
    video_memory[offset + 1] = 0x0A; // Green text
    cursor_x++;
    if (cursor_x >= 80) { cursor_x = 0; cursor_y++; }
}

// --- Bare-Metal String Utilities ---
int kstrcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void print_string(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

void clear_screen() {
    volatile char* video_memory = (volatile char*)0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x0A; // Green text on black
    }
    cursor_x = 0;
    cursor_y = 0;
}

// --- The Core Command Parser ---
void execute_command() {
    // Null-terminate the string so we can read it safely
    command_buffer[buffer_index] = '\0';
    
    // Move to the next line to print the OS response
    cursor_x = 0; 
    cursor_y++;

    // Parse the command
    if (buffer_index > 0) {
        if (kstrcmp(command_buffer, "help") == 0) {
            print_string("PROJECT L SHELL: Commands available -> help, clear");
        } 
        else if (kstrcmp(command_buffer, "clear") == 0) {
            clear_screen();
            // Offset y by -1 because we add 1 at the end of this function
            cursor_y = -1; 
        } 
        else {
            print_string("ERROR: Unknown command '");
            print_string(command_buffer);
            print_string("'");
        }
    }
    
    // Reset the buffer for the next command
    buffer_index = 0;
    cursor_y++;
    cursor_x = 0;
    
    // Print the prompt symbol for the next line
    print_char('>');
}

void handle_backspace() {
    if (buffer_index > 0 && cursor_x > 1) {
        buffer_index--; // Remove from memory buffer
        cursor_x--;     // Move screen cursor back
        
        // Overwrite the character on screen with a blank space
        volatile char* video_memory = (volatile char*)0xB8000;
        int offset = (cursor_y * 80 + cursor_x) * 2;
        video_memory[offset] = ' '; 
    }
}

// --- Main Interrupt Handler ---
extern "C" void keyboard_handler_main() {
    uint8_t status = inb(0x64);
    
    // Check if the buffer is full and data is available
    if (status & 0x01) {
        uint8_t scancode = inb(0x60);
        
        // Ignore key releases (scancodes > 127)
        if (scancode < 128) {
            
            if (scancode == 0x1C) {
                // 'Enter' Key Pressed
                execute_command();
            } 
            else if (scancode == 0x0E) {
                // 'Backspace' Key Pressed
                handle_backspace();
            } 
            else {
                // Regular character key pressed
                char c = kbd_US[scancode];
                if (c != 0 && buffer_index < 255) {
                    command_buffer[buffer_index] = c;
                    buffer_index++;
                    print_char(c);
                }
            }
        }
    }
    
    // Tell the PIC we are done handling the interrupt so it can send more
    outb(0x20, 0x20);
}
