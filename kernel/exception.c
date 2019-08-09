#include "exception.h"
#include "isr.h"
#include "panic.h"
#include "idt.h"
#include "log.h"

void exception_init(void)
{
	idt_encode_entry(idt[0], &exception_divide_by_zero, IDT_RING_ZERO | IDT_PRESENT | IDT_INTERRUPT_GATE);
	idt_encode_entry(idt[4], &exception_overflow, IDT_RING_ZERO | IDT_PRESENT | IDT_INTERRUPT_GATE);	
	idt_encode_entry(idt[5], &exception_bound_range_exceeded, IDT_RING_ZERO | IDT_PRESENT | IDT_INTERRUPT_GATE);	
	idt_encode_entry(idt[6], &exception_invalid_opcode, IDT_RING_ZERO | IDT_PRESENT | IDT_INTERRUPT_GATE);	
	idt_encode_entry(idt[7], &exception_device_not_available, IDT_RING_ZERO | IDT_PRESENT | IDT_INTERRUPT_GATE);	
	idt_encode_entry(idt[8], &exception_double_fault, IDT_RING_ZERO | IDT_PRESENT | IDT_INTERRUPT_GATE);
	idt_encode_entry(idt[14], &exception_page_fault, IDT_RING_ZERO | IDT_PRESENT | IDT_INTERRUPT_GATE);

	log(LOG_OK, "Exception module initialized\n");
}

__attribute__((interrupt))
void exception_divide_by_zero(struct isr_interrupt_frame* frame, uint32_t error_code)
{
	panic_exception(0, frame, error_code);
}

__attribute__((interrupt))
void exception_overflow(struct isr_interrupt_frame* frame, uint32_t error_code)
{
	panic_exception(4, frame, error_code);
}

__attribute__((interrupt))
void exception_bound_range_exceeded(struct isr_interrupt_frame* frame, uint32_t error_code)
{
	panic_exception(5, frame, error_code);
}

__attribute__((interrupt))
void exception_invalid_opcode(struct isr_interrupt_frame* frame, uint32_t error_code)
{
	panic_exception(6, frame, error_code);
}

__attribute__((interrupt))
void exception_device_not_available(struct isr_interrupt_frame* frame, uint32_t error_code)
{
	panic_exception(7, frame, error_code);
}

__attribute__((interrupt))
void exception_double_fault(struct isr_interrupt_frame* frame, uint32_t error_code)
{
	panic_exception(8, frame, error_code);
}

__attribute__((interrupt))
void exception_page_fault(struct isr_interrupt_frame* frame, uint32_t error_code)
{
	panic_exception(14, frame, error_code);
}
