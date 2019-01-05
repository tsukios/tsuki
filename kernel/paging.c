#include "paging.h"
#include "panic.h"
#include "kernel.h"
#include "log.h"
#include "serial.h"

void paging_init(void)
{
	// Identity page the first MiB
	for (unsigned int address = 0; address < 1024*1024; address += 4096)
		paging_map(address, address);

	// Identity map kernel
	paging_allocate_page(((uint32_t) &kernel_end - (uint32_t) &kernel_start) / 4096 + 1);

	paging_enable();

	log(LOG_OK, "Paging module initialized\n");
}

void* paging_allocate_frame(size_t frames)
{
	if (frames == 0)
		PANIC("Tried to allocate zero frames\n");

	// 1MiB offset
	unsigned int index = 256;

find_empty_frame:
	// Find free frame
	while (page_frame_bitmap[index/8] & (index << index%8)) {
		if (++index == 131072 * 8)
			PANIC("Couldn't allocate frame, possibly ran out of memory\n");
	}

	if (frames > 1) {
		unsigned int start = index; // Store index of start of block
		size_t consecutive = 0;

		// Count consecutive free frames
		while (!(page_frame_bitmap[index/8] & (1 << index%8))) {
			if (++consecutive == frames)
				break;
			if (++index == 131072 * 8)
				PANIC("Couldn't allocate frame, possibly ran out of memory\n");
		}

		// If there's not enough space, search for another block
		if (!(consecutive == frames)) {
			index++;
			goto find_empty_frame;
		}

		// Mark block as used
		for (unsigned int i = start; i < start+frames; i++)
			page_frame_bitmap[i/8] |= 1 << i%8;

		// Return block physical address
		return (void*) (start * 4096);
	} else {
		// Mark as used
		page_frame_bitmap[index/8] |= 1 << index%8;

		// Return frame physical address
		return (void*) (index * 4096);
	}
}

void paging_free_frame(void* frame, size_t frames)
{
	if (frames == 0)
		PANIC("Tried to free zero frames\n");

	unsigned int index = (unsigned int) frame / 4096;

	if (frames > 1) {
		// Mark block as free
		for (unsigned int i = index; i < index+frames; i++)
			page_frame_bitmap[i/8] &= ~(1 << i%8);
	} else {
		// Mark as free
		page_frame_bitmap[index/8] &= ~(1 << index%8);
	}
}

void* paging_allocate_page(size_t pages)
{
	if (pages == 0)
		PANIC("Tried to allocate zero pages\n");

	void* pointer = paging_allocate_frame(pages);
	unsigned int address = (unsigned int) pointer;

	// Identity map pages
	for (; pages > 0; pages--, address += 4096)
		paging_map(address, address);

	return pointer;
}

void paging_free_page(void* pointer, size_t pages)
{
	if (pages == 0)
		PANIC("Tried to free zero pages\n");

	unsigned int address = (unsigned int) pointer;

	paging_free_frame(pointer, pages);

	// Mark PTEs as not present
	for (; pages > 0; pages--, address += 4096)
		page_tables[address / (4*1024*1024)][(address / (4*1024)) % 1024].present = 0;
}

void paging_map(unsigned int physical, unsigned int virtual)
{
	// Ensure addresses are 4KiB-aligned
	if ((virtual & 0xFFF) != 0)
		PANIC("Invalid virtual address specified for mapping\n");

	if ((physical & 0xFFF) != 0)
		PANIC("Invalid physical address specified for mapping\n");

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