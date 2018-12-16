#include <string.h>
#include "io.h"
#include "serial.h"

void serial_init(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	outb(port + 1, 0x00); // Disable interrupts
	outb(port + 3, 0x80); // Enable DLAB
	outb(port + 0, 0x03); // (lo) Set divisor to 3
	outb(port + 1, 0x00); // (hi)
	outb(port + 3, 0x03); // Disable DLAB, set parity
	outb(port + 2, 0xC7); // Enable FIFO and clear
	outb(port + 4, 0x0B); // Enable IRQ, RTS/DSR set
}

int serial_received(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	return inb(port + 5) & 1;
}

char serial_read(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	while (!serial_received(port_idx));
	return inb(port);
}

int serial_transmit_empty(int port_idx)
{
	uint16_t port = SERIAL_PORT[port_idx];
	return inb(port + 5) & 0x20;
}

void serial_write(int port_idx, char c)
{
	uint16_t port = SERIAL_PORT[port_idx];
	while (!serial_transmit_empty(port_idx));
	outb(port, c);
}

void serial_writestring(int port_idx, const char* data)
{
	for (size_t i = 0; i < strlen(data); i++) {
		if (data[i] == '\n')
			serial_write(port_idx, '\r');
		serial_write(port_idx, data[i]);
	}
}