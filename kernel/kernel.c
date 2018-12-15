#include "terminal.h"

void kernel_main(void)
{
	terminal_init();
	terminal_writestring("TsukiOS\nline 2");
}