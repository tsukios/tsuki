#include "terminal.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "8259.h"
#include "exception.h"
#include "keyboard.h"
#include "log.h"

void kernel_main(void)
{
	serial_init(0);
	terminal_init();
	gdt_init();
	pic_init();
	exception_init();
	keyboard_init();
	idt_init();

	__asm__ ( "sti" );

	log(LOG_INFO, "All modules loaded\n");

	while (1);

	__asm__ (
		"movl $1, %eax\n\t"
		"movl $0, %ecx\n\t"
		"div %ecx" );
}