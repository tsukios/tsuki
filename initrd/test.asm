[bits 32]

mov eax, 0x1
mov ebx, epic_string

int 0x80

jmp $

epic_string: db "hello from userland!"