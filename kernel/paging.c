#include "paging.h"
#include "panic.h"
#include "kernel.h"
#include "log.h"
#include "serial.h"

#include <stdlib.h>

void paging_init(void)
{
	// Identity page the first MiB
	for (unsigned int address = 0; address < 1024*1024; address += 4096)
		paging_map(address, address);

	// Identity map kernel
	paging_map_kernel();

	paging_enable();

	log(LOG_OK, "Paging module initialized\n");
}

void* paging_allocate_frame(void)
{
	unsigned int i = 256;

	// Find free frame
	while (page_frame_bitmap[i/8] & (1 << i%8)) {
		i++;
		if (i == 131072 * 8)
			panic("Couldn't allocate frame, possibly ran out of memory\n");
	}

	// Mark as used
	page_frame_bitmap[i/8] |= 1 << i%8;

	// Return frame physical address
	return (void*) (i * 4096);
}

void paging_free_frame(void* frame)
{
	unsigned int address = (unsigned int) frame / 4096;

	// Mark as free
	page_frame_bitmap[address/8] &= ~(1 << address%8);
}

void* paging_allocate_page(void)
{
	void* pointer = paging_allocate_frame();
	unsigned int address = (unsigned int) pointer;

	paging_map(address, address);

	return pointer;
}

void paging_free_page(void* pointer)
{
	paging_free_frame(pointer);
	unsigned int address = (unsigned int) pointer;

	page_tables[address / (4*1024*1024)][(address / (4*1024)) % 1024].present = 0;
}

void paging_map(unsigned int physical, unsigned int virtual)
{
	// Ensure addresses are 4KiB-aligned
	if ((virtual & 0xFFF) != 0)
		panic("Invalid virtual address specified for mapping\n");

	if ((physical & 0xFFF) != 0)
		panic("Invalid physical address specified for mapping\n");

	// Mark PDE as present if it isn't already
	struct page_directory_entry* pde = &page_directory[virtual / (4*1024*1024)];
	if (!pde->present) {
		pde->address = (unsigned int) &page_tables[virtual / (4*1024*1024)] >> 12;
		pde->present = 1;
	}

	// Map PTE and mark as present
	struct page_table_entry* pte = &page_tables[virtual / (4*1024*1024)][(virtual / (4*1024)) % 1024];
	pte->address = physical >> 12;
	pte->present = 1;
}

void paging_map_kernel(void)
{
	uint32_t address = (uint32_t) &kernel_start;
	uint32_t end = (uint32_t) &kernel_end;

	while (address < end) {
		paging_allocate_page();
		address += 4096;
	}
}