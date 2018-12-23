#include "paging.h"
#include "panic.h"
#include "log.h"

void paging_init(void)
{
	// Identity page the first ~MiB~ 4MiB
	paging_identity(page_tables[0], 0, /*256*/ 1024);

	page_directory[0].address = ((unsigned int) page_tables[0]) >> 12;
	page_directory[0].present = 1;

	paging_enable();

	log(LOG_OK, "Paging module initialized\n");
}

void paging_identity(page_table_t page_table, uint32_t address, int pages)
{
	for (int i = 0; pages > 0; i++, address += 4096, pages -= 1) {
		page_table[i].address = address >> 12;
		page_table[i].present = 1;
	}
}