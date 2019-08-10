#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

struct syscall_registers {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
};

extern void syscall_handle_wrapper(void);
void syscall_init(void);
void syscall_handle(const struct syscall_registers* registers);

#endif