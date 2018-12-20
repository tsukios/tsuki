#include <stdint.h>
#include "keyboard.h"
#include "io.h"
#include "log.h"

#define PORT 0x60

void keyboard_init(void)
{
	log(LOG_OK, "PS/2 keyboard module initialized\n");
}

uint8_t keyboard_read_scan_code(void)
{
	return io_inb(PORT);
}

char keyboard_to_ascii(enum keyboard_keycode code)
{
	if (code == ENTER_PRESSED) return '\n';
	if (code == BACKSPACE_PRESSED) return '\b';
	if (code == SPACE_PRESSED) return ' ';
	if (code >= 0x10 && code <= 0x1C)
		return "qwertyuiop"[code - 0x10];
	else if (code >= 0x1E && code <= 0x26)
		return "asdfghjkl"[code - 0x1E];
	else if (code >= 0x2C && code <= 0x32)
		return "zxcvbnm"[code - 0x2C];
	return 0;
}