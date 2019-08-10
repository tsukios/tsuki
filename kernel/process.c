#include "process.h"
#include "liballoc.h"
#include "paging.h"

#include <string.h>

struct process* process_spawn(uint8_t* code, unsigned int size)
{
    struct process* process = (struct process*) malloc(sizeof(struct process));
    // Fill in process info & allocate pages
    process->code_page_count = size / 4096 + 1;
    // Put code at 1GiB
    process->code_page = paging_allocate_page(process->code_page_count, 0x40000000, PAGING_USER_MODE | PAGING_READ_WRITE);
    process->stack_page_count = 1;
    // Put stack top at 4GiB
    process->stack_page = paging_allocate_page(process->stack_page_count, (unsigned int) -4096, PAGING_USER_MODE | PAGING_READ_WRITE);
    // Copy code to process
    memcpy(process->code_page, (char*) code, size);
    return process;
}

void process_jump(struct process* process)
{
    process_user_mode_jump((uint32_t) process->code_page, ((uint32_t) process->stack_page) + 4096 - 4);
}