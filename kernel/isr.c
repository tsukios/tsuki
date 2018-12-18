#include "isr.h"
#include "terminal.h"

void isr_handle(void)
{
	terminal_writestring("I");
}