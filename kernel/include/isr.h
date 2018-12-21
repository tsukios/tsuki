#ifndef ISR_H
#define ISR_H

#include <stdint.h>

struct isr_interrupt_frame {
	uint16_t ip;
	uint16_t cs;
	uint16_t flags;
	uint16_t sp;
	uint16_t ss;
};

__attribute__((interrupt)) void isr_hardware_handle(struct isr_interrupt_frame* frame);
__attribute__((interrupt)) void isr_software_handle(struct isr_interrupt_frame* frame);

#endif