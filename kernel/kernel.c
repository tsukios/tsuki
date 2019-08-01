#include "terminal.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "exception.h"
#include "keyboard.h"
#include "paging.h"
#include "multiboot.h"
#include "panic.h"
#include "kernel.h"
#include "log.h"
#include "format.h"
#include "liballoc.h"
#include "tar.h"

void kernel_main(multiboot_info_t* info)
{
	serial_init(0);
	terminal_init();
	paging_init();
	gdt_init();
	pic_init();
	exception_init();
	keyboard_init();
	// IDT is last so that the other modules can register
	// interrupt handlers before the IDT is loaded
	idt_init();

	if (info->flags & MULTIBOOT_INFO_MEM_MAP) {
		log(LOG_INFO, "Memory map found\n");

		unsigned int available = 0;
		multiboot_memory_map_t* map = (multiboot_memory_map_t*) info->mmap_addr;
		while ((unsigned int) map < info->mmap_addr + info->mmap_length) {
			//log(LOG_INFO, "addr=%x, len=%x, type=%x\n", map->addr_low, map->len_low, map->type);
			if (map->type == 1)
				available += map->len_low;
			map = (multiboot_memory_map_t*) ((unsigned int) map + map->size + sizeof(map->size));
		}

		log(LOG_INFO, "Available memory: %dMiB\n", available / 1024 / 1024);
	} else {
		log(LOG_ERROR, "No memory map found\n");
	}

	log(LOG_INFO, "&kernel_start=%x, &kernel_end=%x\n", &kernel_start, &kernel_end);
	
	for (unsigned int index = 0; index < info->mods_count; index++) {
		multiboot_module_t* module = (multiboot_module_t*) ((unsigned int) info->mods_addr + index);
		log(LOG_INFO, "Module %d: %x to %x\n", index, module->mod_start, module->mod_end);

		paging_allocate_page((module->mod_end - module->mod_start) / 4096 + 1);
		unsigned int count = tar_count_headers(module->mod_start);
		log(LOG_INFO, "tar: Headers found: %d\n", count);
		struct tar_header** headers = tar_parse_headers(module->mod_start, count);
		for (unsigned int index = 0; index < count; index++) {
			struct tar_header* header = headers[index];
			log(LOG_INFO, "tar: File %d Name: %s\n", index, header->name);
			if (index == 1)
				((void (*)(void)) tar_get_content(module->mod_start, header))();
			log(LOG_INFO, "tar: File %d Content: %s\n", index, tar_get_content(module->mod_start, header));
		}
	}

	__asm__ ( "sti" );

	log(LOG_INFO, "All modules loaded\n");

	// Test page fault
	//unsigned char* ptr = (unsigned char*) 0x52A000;
	//*ptr = 'a';

	while (1);

	// Test division by zero
	/*__asm__ (
		"movl $1, %eax\n\t"
		"movl $0, %ecx\n\t"
		"div %ecx" );*/
}