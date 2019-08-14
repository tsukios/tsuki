#include "syscall.h"
#include "idt.h"
#include "log.h"
#include "vfs.h"

#include <stdint.h>
#include <stddef.h>

void syscall_init(void)
{
	idt_encode_entry(idt[128], &syscall_handle_wrapper, IDT_RING_THREE | IDT_PRESENT | IDT_INTERRUPT_GATE);

	log(LOG_OK, "Syscall module initialized\n");
}

void syscall_handle(struct syscall_registers* registers)
{
	switch (registers->eax) {
		// print (temporary)
		case 0x01:
			ADDR_CHECK(registers->ebx);

			log(LOG_NONE, "%s", registers->ebx);

			registers->eax = 0;
			break;
		// vfs_open
		case 0x10:
			ADDR_CHECK(registers->ebx);
			ADDR_CHECK(registers->ecx);

			registers->eax = (uint32_t) vfs_open(
				(struct vfs_node*) registers->ebx,
				(char*) registers->ecx);
			break;
		// vfs_close
		case 0x11:
			ADDR_CHECK(registers->ebx);

			registers->eax = (uint32_t) vfs_close(
				(struct vfs_node*) registers->ebx);
			break;
		// vfs_read
		case 0x12:
			ADDR_CHECK(registers->ebx);
			ADDR_CHECK(registers->ecx);

			registers->eax = (uint32_t) vfs_read(
				(struct vfs_node*) registers->ebx,
				(uint8_t*) registers->ecx,
				(uint32_t) registers->edx);
			break;
		// vfs_write
		case 0x13:
			ADDR_CHECK(registers->ebx);
			ADDR_CHECK(registers->ecx);

			registers->eax = (uint32_t) vfs_write(
				(struct vfs_node*) registers->ebx,
				(uint8_t*) registers->ecx,
				(uint32_t) registers->edx);
			break;
		default:
			registers->eax = (uint32_t) SYSCALL_UNKNOWN;
			break;
	}
}