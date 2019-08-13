#include "initrd.h"
#include "log.h"
#include "panic.h"
#include "paging.h"
#include "tar.h"
#include "vfs.h"

#include <stdint.h>
#include <string.h>

void initrd_init(multiboot_info_t* info)
{
    initrd_find(info);

    if ((uint32_t) initrd == 0)
        PANIC("Couldn't find initrd\n");
    
    uint32_t device = vfs_register_device("initrd", &tar_fs, initrd_read, initrd_write);
    vfs_mount("/", device);

    log(LOG_OK, "Initrd module initialized\n");
}

void initrd_find(multiboot_info_t* info)
{
    if (info->mods_count != 1)
        PANIC("Unexpected GRUB module count\n");
    
    multiboot_module_t* module = (multiboot_module_t*) info->mods_addr;
    paging_allocate_page((module->mod_end - module->mod_start) / 4096 + 1, 0, PAGING_IDENTITY_MAP);
    initrd = (void*) module->mod_start;
    initrd_size = module->mod_end - module->mod_start;
    //struct process* process = process_spawn(tar_get_content(module->mod_start, header), tar_parse_size(header->size));
    //log(LOG_INFO, "%x %x %x %x\n", (uint32_t) process->code_page, process->code_page_count, (uint32_t) process->stack_page, process->stack_page_count);
    //process_jump(process);
}

enum error_code initrd_read(uint8_t* buffer, uint32_t offset, uint32_t len)
{
    if (offset > initrd_size ||
        len > initrd_size ||
        offset + len > initrd_size)
        return ERROR;

    memcpy((void*) buffer, (const void*) ((uint32_t) initrd + offset), len);

    return OK;
}

enum error_code initrd_write(uint8_t* buffer, uint32_t offset, uint32_t len)
{
    return VFS_NO_WRITE;
}