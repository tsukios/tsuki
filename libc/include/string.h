#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
char* strncpy(char* dest, const char* src, size_t n);
void* memset(void* str, int c, size_t n);

#endif