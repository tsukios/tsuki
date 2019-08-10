section .text
    global syscall_handle_wrapper
    syscall_handle_wrapper:
        ; push registers to stack and a pointer
        pushad
        push esp
        ; call C function with the pointer
        cld
        extern syscall_handle
        call syscall_handle
        ; restore registers
        add esp, 4
        popad
        iret