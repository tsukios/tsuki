#include "terminal.h"
#include "serial.h"

void kernel_main(void)
{
	terminal_init();
	serial_init(0);

	terminal_writestring("TsukiOS\n");
	serial_writestring(0, "TsukiOS\nSerial port test\n");
}