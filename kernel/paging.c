#include "paging.h"
#include "panic.h"
#include "kernel.h"
#include "log.h"
#include "serial.h"

#include <stdlib.h>

void paging_init(void)
{
	// Identity page the first MiB
	paging_identity(page_tables[0], 0, 256);

	page_directory[0].address = ((unsigned int) page_tables[0]) >> 12;
	page_directory[0].present = 1;

	// Identity map kernel
	paging_map_kernel();

	paging_enable();

	log(LOG_OK, "Paging module initialized\n");
}

void paging_identity(page_table_t page_table, uint32_t address, unsigned int pages)
{
	for (int i = 0; pages > 0; i++, address += 4096, pages -= 1) {
		page_table[i].address = address >> 12;
		page_table[i].present = 1;
	}
}

void paging_map_kernel(void)
{
	uint32_t address = (uint32_t) &kernel_start;
	uint32_t end = (uint32_t) &kernel_end;

	unsigned int page_directory_index = 0;
	unsigned int page_table_index = 256;

	// Mark PDE as present
	page_directory[page_directory_index].address = ((unsigned int) page_tables[page_directory_index]) >> 12;
	page_directory[page_directory_index].present = 1;

	while (address < end) {
		// If reached end of currrent PDE
		if (page_table_index == 1024) {
			// Go to the next PDE
			page_directory_index++;
			page_table_index = 0;

			// Mark PDE as present
			page_directory[page_directory_index].address = ((unsigned int) page_tables[page_directory_index]) >> 12;
			page_directory[page_directory_index].present = 1;
		}

		// Mark PTE as present
		page_tables[page_directory_index][page_table_index].address = address >> 12;
		page_tables[page_directory_index][page_table_index].present = 1;

		// Go to the next page
		address += 4096;
		page_table_index++;
	}
}