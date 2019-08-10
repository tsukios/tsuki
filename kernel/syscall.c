#include "syscall.h"
#include "idt.h"
#include "log.h"

#include <stdint.h>

void syscall_init(void)
{
	idt_encode_entry(idt[128], &syscall_handle_wrapper, IDT_RING_THREE | IDT_PRESENT | IDT_INTERRUPT_GATE);

	log(LOG_OK, "Syscall module initialized\n");
}

void syscall_handle(const struct syscall_registers* registers)
{
	switch (registers->eax) {
		case 0x1:
			log(LOG_NONE, "%s", registers->ebx + 1024*1024*1024);
			break;
		default:
			log(LOG_NONE, "WTF");
			break;
	}
}