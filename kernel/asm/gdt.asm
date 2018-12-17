section .text
	global gdt_flush
	gdt_flush:
		mov eax, DWORD [esp+4]
		lgdt DWORD [eax]
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		jmp 0x08:reload_cs
	reload_cs:
		ret