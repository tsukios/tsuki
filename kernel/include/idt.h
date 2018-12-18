#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <stddef.h>

struct idt_pointer {
	uint16_t size;
	uint32_t offset;
} __attribute__((__packed__));

struct idt_pointer idt_pointer;
uint8_t idt[256][8];

extern void idt_flush(size_t pointer);
extern void idt_test(void);
void idt_init(void);
void idt_encode_entry(uint8_t* target, uint32_t address, uint8_t type_attr);

#endif