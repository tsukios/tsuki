#ifndef PANIC_H
#define PANIC_H

#include "isr.h"

void panic(const char* message);
void panic_exception(const char* name, int vec, struct isr_interrupt_frame* frame);

#endif