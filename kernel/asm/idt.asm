section .text
	global idt_flush
	idt_flush:
		mov eax, DWORD [esp+4]
		lidt DWORD [eax]
		ret

	; remove this later
	global idt_test
	idt_test:
		int 0x80
		ret