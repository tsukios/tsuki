section .text
	global paging_enable
	paging_enable:
		extern page_directory
		mov eax, page_directory
		mov cr3, eax
		mov eax, cr0
		; set paging and protection bits
		or eax, 0x80000001
		mov cr0, eax
		ret