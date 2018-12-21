#include "idt.h"
#include "isr.h"
#include "exception.h"
#include "log.h"

#include <stdint.h>
#include <stddef.h>

void idt_init(void)
{
	// Fill IDT with handler functions
	for (int i = 0; i < 256; i++) {
		// Skip non-empty entries
		if (idt[i][2] != 0)
			continue;
		
		void (*address)(struct isr_interrupt_frame*);
		uint8_t type_attr = RING_ZERO | PRESENT | INTERRUPT_GATE;

		// Add generic handlers
		// Use isr_hardware_handle if vector is for IRQs, software otherwise
		if ((i >= 0x20 && i <= 0x2F) || (i >= 0x70 && i <= 0x77)) {
			address = &isr_hardware_handle;
		} else {
			address = &isr_software_handle;
		}

		idt_encode_entry(idt[i], address, type_attr);
	}

	idt_pointer.offset = (uint32_t) &idt;
	idt_pointer.size = sizeof(idt) - 1;

	idt_flush((size_t) &idt_pointer);

	__asm__ ( "int $0x80" );

	log(LOG_OK, "IDT module initialized\n");
}

void idt_encode_entry(uint8_t* target, void (*func)(struct isr_interrupt_frame*), uint8_t type_attr)
{
	uint32_t address = (uint32_t) func;

	target[0] = address & 0xFF;
	target[1] = (address >> 8) & 0xFF;
	target[6] = (address >> 16) & 0xFF;
	target[7] = (address >> 24) & 0xFF;

	target[5] = type_attr;

	// Point to kernel code selector...
	target[2] = 8;
	target[3] = 0;

	// ...from GDT
	target[4] = 0;
}