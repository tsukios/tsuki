#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

static const uint16_t SERIAL_PORT[4] = {0x3F8, 0x2F8, 0x3E8, 0x2E8};

void serial_init(int port_idx);
int serial_received(int port_idx);
char serial_read(int port_idx);
int serial_transmit_empty(int port_idx);
void serial_putchar(int port_idx, char c);
void serial_writestring(int port_idx, const char* data);

#endif