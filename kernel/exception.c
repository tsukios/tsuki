#include "exception.h"
#include "isr.h"
#include "panic.h"

__attribute__((interrupt))
void exception_divide_by_zero(struct isr_interrupt_frame* frame)
{
	panic_exception("DIVIDE_BY_ZERO", 0x00, frame);
}
