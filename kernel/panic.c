#include <stdlib.h>
#include "panic.h"
#include "isr.h"
#include "serial.h"
#include "terminal.h"

static inline void dump(const char* name, unsigned int value)
{
	char buffer[12];
	itoa(value, buffer, 16);

	terminal_writestring(name);
	terminal_putchar('=');
	terminal_writestring(buffer);
	terminal_putchar('\n');

	serial_writestring(0, name);
	serial_putchar(0, '=');
	serial_writestring(0, buffer);
	serial_putchar(0, '\n');
}

void panic(const char* message)
{
	__asm__ ( "cli" );

	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
	terminal_writestring("\n\n *** KERNEL PANIC *** \n");
	serial_writestring(0, "\n\n *** KERNEL PANIC *** \n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));

	terminal_writestring(message);
	serial_writestring(0, message);

	while (1)
		__asm__ ( "hlt" );
}

void panic_exception(int vec, struct isr_interrupt_frame* frame)
{
	__asm__ ( "cli" );

	const char* name = EXCEPTION_NAMES[vec];

	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
	terminal_writestring("\n\n *** KERNEL PANIC *** \n");
	serial_writestring(0, "\n\n *** KERNEL PANIC *** \n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

	terminal_writestring("  Caused by exception ");
	terminal_writestring(name);
	serial_writestring(0, "  Caused by exception ");
	serial_writestring(0, name);

	if (vec == 15 || vec == 31 || (vec >= 21 && vec <= 29)) {
		terminal_writestring("\n  This exception doesn't exist, it was most likely intentionally invoked through software");
		serial_writestring(0, "\n  This exception doesn't exist, it was most likely intentionally invoked through software");
	} else if (vec == 9) {
		terminal_writestring("\n  This is a legacy exception, it only happens in outdated hardware");
		serial_writestring(0, "\n  This is a legacy exception, it only happens in outdated hardware");
	}

	terminal_writestring("\n  Frame dump:\n");
	serial_writestring(0, "\n  Frame dump:\n");

	dump("    INT", vec);
	dump("    IP", frame->ip);
	dump("    CS", frame->cs);
	dump("    FLAGS", frame->flags);
	dump("    SP", frame->sp);
	dump("    SS", frame->ss);

	while (1)
		__asm__ ( "hlt" );
}