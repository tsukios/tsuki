#include "exception.h"
#include "isr.h"
#include "panic.h"
#include "idt.h"
#include "log.h"

void exception_init(void)
{
	idt_encode_entry(idt[0], &exception_divide_by_zero, RING_ZERO | PRESENT | INTERRUPT_GATE);
	log(LOG_OK, "Exception module initialized\n");
}

__attribute__((interrupt))
void exception_divide_by_zero(struct isr_interrupt_frame* frame)
{
	panic_exception(0x00, frame);
}
