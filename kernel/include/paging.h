#ifndef PAGING_H
#define PAGING_H

#include <stddef.h>
#include <stdint.h>

#define PAGING_IDENTITY_MAP 0x01
#define PAGING_READ_WRITE 0x02
#define PAGING_USER_MODE 0x04

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

typedef struct page_directory_entry page_directory_t[1024] __attribute__((aligned(4096)));
typedef struct page_table_entry page_table_t[1024] __attribute__((aligned(4096)));

page_directory_t page_directory;
page_table_t page_tables[1024]; // This takes up 4MiB of memory, probably should be allocated dynamically in the future
uint8_t page_frame_bitmap[131072]; // 131072 * 8 (bits in byte) * 4KiB (page size) = 4GiB (accessible memory)

extern void paging_enable(void);
void paging_init(void);
void* paging_allocate_frame(size_t frames);
void paging_free_frame(void* frame, size_t frames);
void* paging_allocate_page(uint32_t pages, uint32_t virtual, uint32_t flags);
void paging_free_page(void* pointer, size_t pages);
void paging_map(uint32_t physical, uint32_t virtual, uint32_t flags);

#endif