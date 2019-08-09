#ifndef PANIC_H
#define PANIC_H

#include "isr.h"

#define PANIC(message) panic(message, __FILE__, __LINE__)

static const char *EXCEPTION_NAMES[] = {
	"DIVIDE_BY_ZERO",
	"DEBUG",
	"NON_MASKABLE_INTERRUPT",
	"BREAKPOINT",
	"OVERFLOW",
	"BOUND_RANGE_EXCEEDED",
	"INVALID_OPCODE",
	"DEVICE_NOT_AVAILABLE",
	"DOUBLE_FAULT",
	"COPROCESSOR_SEGMENT_OVERRUN",
	"INVALID_TSS",
	"SEGMENT_NOT_PRESENT",
	"STACK_SEGMENT_FAULT",
	"GENERAL_PROTECTION_FAULT",
	"PAGE_FAULT",
	"UNKNOWN",
	"X87_FLOATING_POINT_EXCEPTION",
	"ALIGNMENT_CHECK",
	"MACHINE_CHECK",
	"SIMD_FLOATING_POINT_EXCEPTION",
	"VIRTUALIZATION_EXCEPTION",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"SECURITY_EXCEPTION",
	"UNKNOWN",
};

__attribute__((noreturn)) void panic(const char* message, const char* file, unsigned int line);
__attribute__((noreturn)) void panic_exception(int vec, struct isr_interrupt_frame* frame, uint32_t error_code);

#endif