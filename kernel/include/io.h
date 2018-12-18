#ifndef IO_H
#define IO_H 

#include <stdint.h>

extern uint8_t io_inb(uint16_t port);
extern void io_outb(uint16_t port, uint8_t value);

#endif