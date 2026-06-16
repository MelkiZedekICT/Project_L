#ifndef KSTRING_H
#define KSTRING_H

#include <stddef.h>
#ifdef __cplusplus

extern "C" {
#endif

size_t kstrlen(const char* str);
int kstrcmp(const char* s1, const char* s2);
void* kmemcpy(void* dest, const void* src, size_t n);

/**
 * Fills the first n bytes of memory with the given byte value.
 * Returns dest.
 */
void* kmemset(void* dest, int value, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* KSTRING_H */
