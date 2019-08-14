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
#include "process.h"
#include "syscall.h"
#include "initrd.h"
#include "vfs.h"
#include "error.h"

void kernel_main(multiboot_info_t* info)
{
	serial_init(0);
	terminal_init();
	paging_init();
	gdt_init();
	pic_init();
	exception_init();
	keyboard_init();
	syscall_init();
	tar_init();
	// Initrd needs to be loaded here otherwise
	// the other modules can accidentally overwrite it
	initrd_init(info);
	vfs_init();
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

	//__asm__ ( "sti" );

	log(LOG_INFO, "All modules loaded\n");

    // Load user mode program
    struct vfs_node node;
    enum error_code ret = vfs_open(&node, "/initrd/test_src/test");
    //log(LOG_INFO, "vfs_open: RET=%d, PATH=%s, SIZE=%d, ACCESS=%d, POS=%d\n",
    //    ret, node.path, node.size, node.access_mode, node.position);

	if (ret != OK)
		PANIC("Couldn't open init program\n");

    uint8_t* buffer = (uint8_t*) malloc(node.size);
    ret = vfs_read(&node, buffer, node.size);
    //log(LOG_INFO, "vfs_read: RET=%d, N=20, BUFFER=%s\n", ret, buffer);

	if (ret != OK)
		PANIC("Couldn't read init program\n");

    struct process* process = process_spawn(buffer, node.size);
    process_jump(process);

    free((void*) buffer);

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