#include "terminal.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "8259.h"

void kernel_main(void)
{
	gdt_init();
	terminal_init();
	serial_init(0);
	idt_init();
	pic_init();

	__asm__ ( "sti" );

	terminal_writestring("TsukiOS\n");
	serial_writestring(0, "TsukiOS\nSerial port test\n");

	__asm__ (
		"movl $1, %eax\n\t"
		"movl $0, %ecx\n\t"
		"div %ecx" );

	while (1);
}