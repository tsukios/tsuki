#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

struct process {
    void* code_page;
    uint8_t code_page_count;
    void* stack_page;
    uint8_t stack_page_count;
};

extern void process_user_mode_jump(uint32_t eip, uint32_t esp);
struct process* process_spawn(uint8_t* code, unsigned int size);
void process_jump(struct process* process);

#endif