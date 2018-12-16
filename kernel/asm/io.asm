section .text
	global inb
	inb:
		mov dx, WORD [esp+4]
		in al, dx
		ret

	global outb
	outb:
		mov dx, WORD [esp+4]
		mov al, BYTE [esp+8]
		out dx, al
		ret