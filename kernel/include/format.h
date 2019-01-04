#ifndef FORMAT_H
#define FORMAT_H

#include "stdarg.h"

void format(char* buffer, const char* string, ...);
void format_inner(char* buffer, const char* string, va_list args);
void format_int(char* buffer, int value, int base);

#endif