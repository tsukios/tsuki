#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "isr.h"

__attribute__((interrupt)) void exception_divide_by_zero(struct isr_interrupt_frame* frame);

#endif