#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

struct page_directory_entry {
	unsigned int present: 1;
	unsigned int read_write: 1;
	unsigned int user_access: 1;
	unsigned int write_through: 1;
	unsigned int cache_disable: 1;
	unsigned int accessed: 1;
	unsigned int zero: 1;
	unsigned int size: 1;
	unsigned int unused: 4;
	unsigned int address: 20;
} __attribute__((packed));

struct page_table_entry {
	unsigned int present: 1;
	unsigned int read_write: 1;
	unsigned int user_access: 1;
	unsigned int write_through: 1;
	unsigned int cache_disable: 1;
	unsigned int accessed: 1;
	unsigned int dirty: 1;
	unsigned int zero: 1;
	unsigned int global: 1;
	unsigned int unused: 3;
	unsigned int address: 20;
} __attribute__((packed));

typedef struct page_directory_entry page_directory_t[1024];
typedef struct page_table_entry page_table_t[1024];

page_directory_t page_directory __attribute__((aligned(4096)));
page_table_t page_tables[1] __attribute__((aligned(4096)));

extern void paging_enable(void);
void paging_init(void);
void paging_identity(page_table_t page_table, uint32_t address, int pages);

#endif