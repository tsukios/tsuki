#ifndef INITRD_H
#define INITRD_H

#include "multiboot.h"

#include <stdint.h>

void* initrd;
uint32_t initrd_size;

void initrd_init(multiboot_info_t* info);
void initrd_find(multiboot_info_t* info);
uint8_t initrd_read(uint8_t* buffer, uint32_t offset, uint32_t len);
uint8_t initrd_write(uint8_t* buffer, uint32_t offset, uint32_t len);

#endif