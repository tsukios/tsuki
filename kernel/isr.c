#include "isr.h"
#include "terminal.h"

__attribute__((interrupt))
void isr_hardware_handle(struct interrupt_frame* frame)
{
	terminal_writestring("[Hardware Interrupt]");
}

__attribute__((interrupt))
void isr_software_handle(struct interrupt_frame* frame)
{
	terminal_writestring("[Software Interrupt]");
}