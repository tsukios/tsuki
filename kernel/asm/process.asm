section .text
    global process_user_mode_jump
    process_user_mode_jump:
        mov eax, DWORD [esp+4]
        mov ebx, DWORD [esp+8]

        push 0x20 | 3 ; ss
        push ebx      ; esp
        pushf         ; eflags
        push 0x18 | 3 ; cs
        push eax      ; eip

        iret