section .text
	global io_inb
	io_inb:
		mov dx, WORD [esp+4]
		in al, dx
		ret

	global io_outb
	io_outb:
		mov dx, WORD [esp+4]
		mov al, BYTE [esp+8]
		out dx, al
		ret