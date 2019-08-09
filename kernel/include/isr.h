#ifndef ISR_H
#define ISR_H

#include <stdint.h>

struct isr_interrupt_frame {
	uint32_t ip;
	uint32_t cs;
	uint32_t flags;
	uint32_t sp;
	uint32_t ss;
};

__attribute__((interrupt)) void isr_hardware_handle(struct isr_interrupt_frame* frame);
__attribute__((interrupt)) void isr_software_handle(struct isr_interrupt_frame* frame);

#endif