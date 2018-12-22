#include "keyboard.h"
#include "io.h"
#include "terminal.h"
#include "pic.h"
#include "isr.h"
#include "idt.h"
#include "log.h"

#include <stdint.h>

void keyboard_init(void)
{
	idt_encode_entry(idt[0x21], &keyboard_isr, IDT_INTERRUPT_GATE | IDT_RING_ZERO | IDT_PRESENT);

	log(LOG_OK, "PS/2 keyboard module initialized\n");
}

uint8_t keyboard_read_scan_code(void)
{
	return io_inb(0x60);
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

__attribute__((interrupt))
void keyboard_isr(struct isr_interrupt_frame* frame)
{
	char key = keyboard_to_ascii(keyboard_read_scan_code());
	if (key)
		terminal_putchar(key);
	pic_send_eoi(1);
}