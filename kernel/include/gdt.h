#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stddef.h>

struct gdt_pointer {
	uint16_t size;
	uint32_t offset;
} __attribute__((__packed__));

struct gdt_pointer gdt_pointer;
uint8_t gdt[3][8];

extern void gdt_flush(size_t pointer);
void gdt_init(void);
void gdt_encode_entry(uint8_t* target, uint32_t base, uint32_t limit, uint8_t type);

#endif