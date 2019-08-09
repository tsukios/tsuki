#include "panic.h"
#include "isr.h"
#include "terminal.h"
#include "log.h"

__attribute__((noreturn))
void panic(const char* message, const char* file, unsigned int line)
{
	__asm__ ( "cli" );

	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
	log(LOG_NONE, "\n\n**** KERNEL PANIC ****\n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

	log(LOG_NONE, "From %s:%d\n", file, line);
	log(LOG_NONE, message);

	while (1)
		__asm__ ( "hlt" );
}

__attribute__((noreturn))
void panic_exception(int vec, struct isr_interrupt_frame* frame, uint32_t error_code)
{
	__asm__ ( "cli" );

	const char* name = EXCEPTION_NAMES[vec];

	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
	log(LOG_NONE, "\n\n**** KERNEL PANIC ****\n");
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

	log(LOG_NONE, "Caused by exception %s\n", name);
	log(LOG_NONE, "Error code: 0x%x\n", error_code);

	if (vec == 15 || vec == 31 || (vec >= 21 && vec <= 29)) {
		log(LOG_NONE, "This exception doesn't exist, it was most likely intentionally invoked through software\n");
	} else if (vec == 9) {
		log(LOG_NONE, "This is a legacy exception, it only happens in outdated hardware\n");
	}

	log(LOG_NONE, "Frame dump:\n");

	log(LOG_NONE, "INT=0x%x\n", vec);
	log(LOG_NONE, "IP=0x%x\n", frame->ip);
	log(LOG_NONE, "CS=0x%x\n", frame->cs);
	log(LOG_NONE, "FLAGS=0x%x\n", frame->flags);
	log(LOG_NONE, "SP=0x%x\n", frame->sp);
	log(LOG_NONE, "SS=0x%x\n", frame->ss);

	while (1)
		__asm__ ( "hlt" );
}