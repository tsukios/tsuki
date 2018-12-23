#include "terminal.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "exception.h"
#include "keyboard.h"
#include "paging.h"
#include "log.h"

void kernel_main(void)
{
	serial_init(0);
	terminal_init();
	gdt_init();
	paging_init();
	pic_init();
	exception_init();
	keyboard_init();
	// IDT is last so that the other modules can register
	// interrupt handlers before the IDT is loaded
	idt_init();

	__asm__ ( "sti" );

	log(LOG_INFO, "All modules loaded\n");

	// Test page fault
	unsigned char* ptr = (unsigned char*) 0x400000;
	*ptr = 'a';

	while (1);

	// Test division by zero
	__asm__ (
		"movl $1, %eax\n\t"
		"movl $0, %ecx\n\t"
		"div %ecx" );
}