#include <stdlib.h>
#include "panic.h"
#include "isr.h"
#include "serial.h"
#include "terminal.h"

void panic(const char* message)
{
	__asm__ ( "cli" );

	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_RED));
	terminal_writestring("     KERNEL PANIC     \n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

	terminal_writestring(message);

	while (1)
		__asm__ ( "hlt" );
}

void panic_exception(const char* name, int vec, struct isr_interrupt_frame* frame)
{
	__asm__ ( "cli" );

	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_RED));
	terminal_writestring("     KERNEL PANIC     \n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

	terminal_writestring("Caused by exception ");
	terminal_writestring(name);
	terminal_writestring("\nFrame dump:\n");

	{
		char buffer[12];
		itoa(vec, buffer, 16);
		terminal_writestring("INT=");
		terminal_writestring(buffer);
		terminal_putchar('\n');
	}

	{
		char buffer[12];
		itoa(frame->cs, buffer, 16);
		terminal_writestring("CS=");
		terminal_writestring(buffer);
		terminal_putchar('\n');
	}
	
	{
		char buffer[12];
		itoa(frame->flags, buffer, 16);
		terminal_writestring("FLAGS=");
		terminal_writestring(buffer);
		terminal_putchar('\n');
	}

	{
		char buffer[12];
		itoa(frame->sp, buffer, 16);
		terminal_writestring("SP=");
		terminal_writestring(buffer);
		terminal_putchar('\n');
	}

	{
		char buffer[12];
		itoa(frame->ss, buffer, 16);
		terminal_writestring("SS=");
		terminal_writestring(buffer);
		terminal_putchar('\n');
	}

	while (1)
		__asm__ ( "hlt" );
}