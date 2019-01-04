#include "isr.h"
#include "terminal.h"
#include "keyboard.h"
#include "pic.h"

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