#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* str, int c, size_t n);

#endif