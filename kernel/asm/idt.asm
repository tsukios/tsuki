section .text
	global idt_flush
	idt_flush:
		mov eax, DWORD [esp+4]
		lidt DWORD [eax]
		ret