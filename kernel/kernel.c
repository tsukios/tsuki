#include "terminal.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"

void kernel_main(void)
{
	gdt_init();
	terminal_init();
	serial_init(0);
	idt_init();

	terminal_writestring("TsukiOS\n");
	serial_writestring(0, "TsukiOS\nSerial port test\n");
}