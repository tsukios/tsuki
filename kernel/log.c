#include "log.h"
#include "terminal.h"
#include "serial.h"
#include "format.h"

#include <stdint.h>
#include <stdarg.h>

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

	char buffer[1024];
	format_inner(buffer, data, args);

	terminal_writestring(buffer);
	serial_writestring(0, buffer);

	va_end(args);
}