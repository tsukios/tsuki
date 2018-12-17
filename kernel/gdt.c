#include "gdt.h"

void gdt_init()
{
	gdt_encode_entry(gdt[0], 0, 0, 0); // null 
	gdt_encode_entry(gdt[1], 0, 0xFFFFFFFF, 0x9A); // kernel code
	gdt_encode_entry(gdt[2], 0, 0xFFFFFFFF, 0x92); // kernel data

	gdt_pointer.offset = (uint32_t) &gdt;
	gdt_pointer.size = sizeof(gdt) - 1;

	gdt_flush((size_t) &gdt_pointer);
}

void gdt_encode_entry(uint8_t* target, uint32_t base, uint32_t limit, uint8_t type)
{
	if ((limit > 0x10000) && (limit & 0xFFF) != 0xFFF) {
		return; // error here
	}

	if (limit > 0x10000) {
		limit >>= 12;
		target[6] = 0xC0;
	} else {
		target[6] = 0x40;
	}

	target[0] = limit & 0xFF;
	target[1] = (limit >> 8) & 0xFF;
	target[6] |= (limit >> 16) & 0xF;

	target[2] = base & 0xFF;
	target[3] = (base >> 8) & 0xFF;
	target[4] = (base >> 16) & 0xFF;
	target[7] = (base >> 24) & 0xFF;

	target[5] = type;
}