#include <stdint.h>
#include "8259.h"
#include "io.h"
#include "log.h"

#define MASTER_COMMAND  0x20
#define MASTER_DATA     0x21
#define SLAVE_COMMAND   0xA0
#define SLAVE_DATA      0xA1

void pic_init(void)
{
	pic_remap(0x20, 0x70);
	pic_clear_masks();

	log(LOG_INFO, "8259 PIC module initialized\n");
}

void pic_send_eoi(uint8_t irq)
{
	if (irq >= 8)
		io_outb(SLAVE_COMMAND, 0x20);

	io_outb(MASTER_COMMAND, 0x20);
}

void pic_remap(uint8_t master_offset, uint8_t slave_offset)
{
	uint8_t master_mask, slave_mask;

	// Save current masks
	master_mask = io_inb(MASTER_DATA);
	slave_mask = io_inb(SLAVE_DATA);

	// Start initialization sequence
	io_outb(MASTER_COMMAND, 0x10 | 0x01);
	io_outb(SLAVE_COMMAND, 0x10 | 0x01);

	// Remap vector offsets for each PIC
	io_outb(MASTER_DATA, master_offset);
	io_outb(SLAVE_DATA, slave_offset);

	// Tell the master there's a slave at IRQ2
	io_outb(MASTER_DATA, 4);
	io_outb(SLAVE_DATA, 2);

	// Enable 8086 mode
	io_outb(MASTER_DATA, 0);
	io_outb(SLAVE_DATA, 0);

	// Restore masks
	io_outb(MASTER_DATA, master_mask);
	io_outb(SLAVE_DATA, slave_mask);
}

void pic_clear_masks(void)
{
	io_outb(MASTER_DATA, 0);
	io_outb(SLAVE_DATA, 0);
}
