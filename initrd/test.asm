[bits 32]

; Print string
mov eax, 0x01
mov ebx, epic_string
int 0x80

; Open file
mov eax, 0x10
mov ebx, node
mov ecx, path
int 0x80

; Read file
mov eax, 0x12
mov ebx, node
mov ecx, buffer
mov edx, 20
int 0x80

; Print file content
mov eax, 0x01
mov ebx, buffer
int 0x80

jmp $

epic_string: db "hello from userland!", 10, "i will now print hello_from_initrd.txt file's content:", 10, 0
path: db "/initrd/hello_from_initrd.txt", 0
node:
    node_path: dd 0
    node_size: dd 0
    node_access_mode: dd 0
    node_position: dd 0
    node_priv: dd 0
buffer: resb 21