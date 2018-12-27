#include "log.h"
#include "terminal.h"
#include "serial.h"
#include "stdint.h"

#include <stdarg.h>
#include <stdlib.h>

void log(enum log_type type, const char* data, ...)
{
	uint8_t color = terminal_getcolor();

	switch (type) {
		case LOG_OK:
			terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
			terminal_writestring("[  OK  ] ");
			serial_writestring(0, "[  OK  ] ");
			break;
		case LOG_INFO:
			terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
			terminal_writestring("[ INFO ] ");
			serial_writestring(0, "[ INFO ] ");
			break;
		case LOG_WARNING:
			terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK));
			terminal_writestring("[ WARN ] ");
			serial_writestring(0, "[ WARN ] ");
			break;
		case LOG_ERROR:
			terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
			terminal_writestring("[FAILED] ");
			serial_writestring(0, "[FAILED] ");
			break;
		default:
			break;
	}

	terminal_setcolor(color);

	va_list args;
	va_start(args, data);

	for (const char* ptr = data; *ptr; ptr++) {
		if (*ptr != '%') {
			terminal_putchar(*ptr);
			serial_putchar(0, *ptr);
			continue;
		}

		ptr++;

		char value;
		char* string_pointer;
		char buffer[12];

		switch (*ptr) {
			case 's':
				string_pointer = (char*) va_arg(args, unsigned int);
				terminal_writestring(string_pointer);
				serial_writestring(0, string_pointer);
				break;
			case 'c':
				value = va_arg(args, int);
				terminal_putchar(value);
				serial_putchar(0, value);
				break;
			case 'x':
				itoa(va_arg(args, unsigned int), buffer, 16);
				terminal_writestring(buffer);
				serial_writestring(0, buffer);
				break;
			case 'd':
				itoa(va_arg(args, unsigned int), buffer, 10);
				terminal_writestring(buffer);
				serial_writestring(0, buffer);
				break;
			case '%':
				terminal_putchar('%');
				serial_putchar(0, '%');
				break;
			default:
				break;
		}
	}

	va_end(args);
}