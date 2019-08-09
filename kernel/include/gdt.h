#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stddef.h>

struct gdt_pointer {
	uint16_t size;
	uint32_t offset;
} __attribute__((__packed__));

struct gdt_tss {
	uint16_t link;
	uint16_t link_h;
	uint32_t esp0;
	uint16_t ss0;
	uint16_t ss0_h;
	uint32_t esp1;
	uint16_t ss1;
	uint16_t ss1_h;
	uint32_t esp2;
	uint16_t ss2;
	uint16_t ss2_h;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;
	uint16_t es_h;
	uint16_t cs;
	uint16_t cs_h;
	uint16_t ss;
	uint16_t ss_h;
	uint16_t ds;
	uint16_t ds_h;
	uint16_t fs;
	uint16_t fs_h;
	uint16_t gs;
	uint16_t gs_h;
	uint16_t ldt;
	uint16_t ldt_h;
	uint16_t trap;
	uint16_t iomap;
} __attribute__((__packed__));

struct gdt_pointer gdt_pointer;
struct gdt_tss gdt_tss;
uint8_t gdt[6][8];

extern void stack_top(void);
extern void gdt_flush(size_t pointer);
extern void gdt_tss_flush(void);
void gdt_init(void);
void gdt_encode_entry(uint8_t target[8], uint32_t base, uint32_t limit, uint8_t type);

#endif