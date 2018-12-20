#include <string.h>
#include "io.h"
#include "serial.h"
#include "log.h"

void serial_init(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	io_outb(port + 1, 0x00); // Disable interrupts
	io_outb(port + 3, 0x80); // Enable DLAB
	io_outb(port + 0, 0x03); // (lo) Set divisor to 3
	io_outb(port + 1, 0x00); // (hi)
	io_outb(port + 3, 0x03); // Disable DLAB, set parity
	io_outb(port + 2, 0xC7); // Enable FIFO and clear
	io_outb(port + 4, 0x0B); // Enable IRQ, RTS/DSR set

	log(LOG_OK, "Serial module initialized\n");
}

int serial_received(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	return io_inb(port + 5) & 1;
}

char serial_read(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	while (!serial_received(port_idx));
	return io_inb(port);
}

int serial_transmit_empty(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	return io_inb(port + 5) & 0x20;
}

void serial_putchar(int port_idx, char c)
{
	uint16_t port = SERIAL_PORT[port_idx];
	while (!serial_transmit_empty(port_idx));
	if (c == '\n')
		serial_putchar(port_idx, '\r');
	io_outb(port, c);
}

void serial_writestring(int port_idx, const char* data)
{
	for (size_t i = 0; i < strlen(data); i++) {
		serial_putchar(port_idx, data[i]);
	}
}