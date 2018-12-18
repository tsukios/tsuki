#include <stdint.h>
#include <stddef.h>
#include "idt.h"
#include "isr.h"

#define INTERRUPT_GATE  0b00001110
#define TRAP_GATE       0b00001111
#define TASK_GATE       0b00010101

#define RING_ZERO       0b00000000
#define RING_THREE      0b01000000

#define PRESENT         0b10000000

void idt_init(void)
{
	// Fill IDT with 256 entries
	for (int i = 0; i < 256; i++) {
		// Use isr_hardware_handle if vector is for IRQs, software otherwise
		uint32_t address;
		if ((i >= 0x20 && i <= 0x2F) || (i >= 0x70 && i <= 0x77))
			address = (uint32_t) &isr_hardware_handle;
		else
			address = (uint32_t) &isr_software_handle;

		idt_encode_entry(idt[i], address, INTERRUPT_GATE | RING_ZERO | PRESENT);
	}

	idt_pointer.offset = (uint32_t) &idt;
	idt_pointer.size = sizeof(idt) - 1;

	idt_flush((size_t) &idt_pointer);

	idt_test();
}

void idt_encode_entry(uint8_t* target, uint32_t address, uint8_t type_attr)
{
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
