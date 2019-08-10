#include "format.h"

#include <stdarg.h>

void format(char* buffer, const char* string, ...)
{
	va_list args;
	va_start(args, string);

	format_inner(buffer, string, args);

	va_end(args);
}

void format_inner(char* buffer, const char* string, va_list args)
{
	// Save start of string and buffer to ptr and buf, respectively
	const char* ptr = string;
	char* buf = buffer;

	// Loop until ptr points to a null character (end of string) 
	for (; *ptr; ptr++, buf++) {
		// If it's not a %, append char to buffer 
		if (*ptr != '%') {
			*buf = *ptr;
			continue;
		}

		// If it is a %, go to char after
		ptr++;

		// These need to be declared outside switch
		char* str;
		char bnt[12];

		switch (*ptr) {
			case 's':
				str = (char*) va_arg(args, unsigned int);
				while (*str)
					*buf++ = *str++;
				buf--;
				break;
			case 'c':
				*buf = (char) va_arg(args, int);
				break;
			case 'x':
				format_int(bnt, va_arg(args, unsigned int), 16);
				// Append bnt to buffer
				for (char* bnt_ptr = bnt; *bnt_ptr; bnt_ptr++)
					*buf++ = *bnt_ptr;
				buf--;
				break;
			case 'd':
				format_int(bnt, va_arg(args, int), 10);
				// Append bnt to buffer
				for (char* bnt_ptr = bnt; *bnt_ptr; bnt_ptr++)
					*buf++ = *bnt_ptr;
				buf--;
				break;
			case 'u':
				format_int(bnt, va_arg(args, int), 10);
				// Append bnt to buffer
				for (char* bnt_ptr = bnt; *bnt_ptr; bnt_ptr++)
					*buf++ = *bnt_ptr;
				buf--;
				break;
			case '%':
				*buf = '%';
				break;
			default:
				break;
		}
	}

	// Append null character to buffer
	*buf = '\0';
}

void format_int(char* buffer, int value, int base)
{
	char* ptr;
	char* low;

	if (base < 2 || base > 36) {
		*buffer = '\0';
	}

	ptr = buffer;

	if (value < 0 && base == 10)
		*ptr++ = '-';

	low = ptr;

	do {
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
		value /= base;
	} while (value);

	*ptr-- = '\0';

	while (low < ptr) {
		char tmp = *low;
		*low++ = *ptr;
		*ptr-- = tmp;
	}
}