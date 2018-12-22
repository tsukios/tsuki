#ifndef IDT_H
#define IDT_H

#include "isr.h"

#include <stdint.h>
#include <stddef.h>

#define IDT_INTERRUPT_GATE  0b00001110
#define IDT_TRAP_GATE       0b00001111
#define IDT_TASK_GATE       0b00010101

#define IDT_RING_ZERO       0b00000000
#define IDT_RING_THREE      0b01000000

#define IDT_PRESENT         0b10000000

struct idt_pointer {
	uint16_t size;
	uint32_t offset;
} __attribute__((__packed__));

struct idt_pointer idt_pointer;
uint8_t idt[256][8];

extern void idt_flush(size_t pointer);
void idt_init(void);
void idt_encode_entry(uint8_t* target, void (*func)(struct isr_interrupt_frame*), uint8_t type_attr);

#endif