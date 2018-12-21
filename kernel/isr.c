#include "isr.h"
#include "terminal.h"
#include "keyboard.h"
#include "8259.h"

#include <stdlib.h>

__attribute__((interrupt))
void isr_hardware_handle(struct isr_interrupt_frame* frame)
{
	terminal_writestring("[H]");
}

__attribute__((interrupt))
void isr_software_handle(struct isr_interrupt_frame* frame)
{
	terminal_writestring("[S]");
}

__attribute__((interrupt))
void isr_keyboard(struct isr_interrupt_frame* frame)
{
	char key = keyboard_to_ascii(keyboard_read_scan_code());
	if (key != 0)
		terminal_putchar(key);
	pic_send_eoi(1);
}