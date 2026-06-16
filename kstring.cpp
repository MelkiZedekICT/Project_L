#include "kstring.h"
#include <stdint.h>   // for uint8_t

/*
 * All functions are placed in an extern "C" block to ensure
 * C‑linkage, making them callable from assembly or C code.
 */
extern "C" {

size_t kstrlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0')
        ++len;
    return len;
}

int kstrcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }
    // Use unsigned char to avoid sign extension issues
    return (unsigned char)*s1 - (unsigned char)*s2;
}

void* kmemcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (size_t i = 0; i < n; ++i)
        d[i] = s[i];
    return dest;
}

void* kmemset(void* dest, int value, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    uint8_t val = (uint8_t)value;
    for (size_t i = 0; i < n; ++i)
        d[i] = val;
    return dest;
}

} /* extern "C" */
