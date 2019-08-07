#include "process.h"
#include "liballoc.h"
#include "paging.h"

#include <string.h>

struct process* process_spawn(uint8_t* code, unsigned int size)
{
    struct process* process = (struct process*) malloc(sizeof(struct process));
    // Fill in process info & allocate pages
    process->code_page_count = size / 4096 + 1;
    process->code_page = paging_allocate_page(process->code_page_count, 1, 0);
    process->stack_page_count = 1;
    process->stack_page = paging_allocate_page(process->stack_page_count, 1, 1);
    // Copy code to process
    strncpy(process->code_page, (char*) code, size);
    return process;
}

void process_jump(struct process* process)
{
    ((void (*)(void)) process->code_page)();
}