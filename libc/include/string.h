#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* str, int c, size_t n);

#endif