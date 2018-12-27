#include "panic.h"
#include "isr.h"
#include "serial.h"
#include "terminal.h"

#include <stdlib.h>

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

__attribute__((noreturn))
void panic(const char* message, const char* file, unsigned int line)
{
	__asm__ ( "cli" );

	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
	terminal_writestring("\n\n**** KERNEL PANIC ****\n");
	serial_writestring(0, "\n\n**** KERNEL PANIC ****\n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

	terminal_writestring("From ");
	serial_writestring(0, "From ");

	terminal_writestring(file);
	serial_writestring(0, file);

	terminal_putchar(':');
	serial_putchar(0, ':');
	
	{
		char buffer[12];
		itoa(line, buffer, 10);
		terminal_writestring(buffer);
		serial_writestring(0, buffer);
	}

	terminal_putchar('\n');
	serial_putchar(0, '\n');

	terminal_writestring(message);
	serial_writestring(0, message);

	while (1)
		__asm__ ( "hlt" );
}

__attribute__((noreturn))
void panic_exception(int vec, struct isr_interrupt_frame* frame)
{
	__asm__ ( "cli" );

	const char* name = EXCEPTION_NAMES[vec];

	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
	terminal_writestring("\n\n**** KERNEL PANIC ****\n");
	serial_writestring(0, "\n\n**** KERNEL PANIC ****\n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

	terminal_writestring("Caused by exception ");
	terminal_writestring(name);
	serial_writestring(0, "Caused by exception ");
	serial_writestring(0, name);

	if (vec == 15 || vec == 31 || (vec >= 21 && vec <= 29)) {
		terminal_writestring("\nThis exception doesn't exist, it was most likely intentionally invoked through software");
		serial_writestring(0, "\nThis exception doesn't exist, it was most likely intentionally invoked through software");
	} else if (vec == 9) {
		terminal_writestring("\nThis is a legacy exception, it only happens in outdated hardware");
		serial_writestring(0, "\nThis is a legacy exception, it only happens in outdated hardware");
	}

	terminal_writestring("\nFrame dump:\n");
	serial_writestring(0, "\nFrame dump:\n");

	dump("INT", vec);
	dump("IP", frame->ip);
	dump("CS", frame->cs);
	dump("FLAGS", frame->flags);
	dump("SP", frame->sp);
	dump("SS", frame->ss);

	while (1)
		__asm__ ( "hlt" );
}