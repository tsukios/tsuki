#include "log.h"
#include "terminal.h"
#include "serial.h"
#include "stdint.h"

void log(enum log_type type, const char* data)
{
	uint8_t color = terminal_getcolor();

	switch (type) {
		case LOG_INFO:
			terminal_setcolor(vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK));
			terminal_writestring("[INFO] ");
			serial_writestring(0, "[INFO] ");
			break;
		case LOG_WARNING:
			terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK));
			terminal_writestring("[WARNING] ");
			serial_writestring(0, "[WARNING] ");
			break;
		case LOG_ERROR:
			terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
			terminal_writestring("[ERROR] ");
			serial_writestring(0, "[ERROR] ");
			break;
		default:
			break;
	}

	terminal_setcolor(color);

	terminal_writestring(data);
	serial_writestring(0, data);
}