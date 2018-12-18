section .text
	global isr_wrapper
	isr_wrapper:
		pushad
		cld
		extern isr_handle
		call isr_handle
		popad
		iret