#ifndef SYSCALL_H
#define SYSCALL_H

#include "error.h"

#include <stdint.h>
#include <stddef.h>

#define ADDR_OFFSET 1024*1024*1024

#define ADDR_CHECK(addr) { \
    enum error_code ret = syscall_check_address((size_t) (addr)); \
    if (ret != OK) { \
        registers->eax = ret; \
        return; \
    } \
}

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

// TODO: make this function check if the address was mapped
inline enum error_code syscall_check_address(size_t address)
{
    if (address >= (size_t) 3*1024*1024*1024)
        return SYSCALL_INVALID_ADDRESS;

    return OK;
}

extern void syscall_handle_wrapper(void);
void syscall_init(void);
void syscall_handle(struct syscall_registers* registers);

#endif