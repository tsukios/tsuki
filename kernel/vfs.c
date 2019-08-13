#include "vfs.h"
#include "log.h"
#include "liballoc.h"
#include "panic.h"

#include <string.h>

void vfs_init(void)
{
    if (strcmp(vfs_mounts[0].path, "/") != 0)
        PANIC("No root mount point found\n");

    log(LOG_OK, "VFS module initialized\n");
}

uint32_t vfs_register_device(
    char* name,
    struct vfs_fs* fs,
    enum error_code (*read)(uint8_t* buffer, uint32_t offset, uint32_t len),
    enum error_code (*write)(uint8_t* buffer, uint32_t offset, uint32_t len))
{
    if (vfs_devices_len == VFS_DEVICE_MAX)
        PANIC("Exceeded VFS device max\n");

    struct vfs_device* device = &vfs_devices[vfs_devices_len++];
    device->id = vfs_generate_device_id();
    device->name = name;
    device->fs = fs;
    device->read = read;
    device->write = write;

    return vfs_devices_len - 1;
}

uint32_t vfs_mount(char* path, uint32_t device)
{
    if (vfs_mounts_len == VFS_MOUNT_MAX)
        PANIC("Exceeded VFS mount max\n");
    
    struct vfs_mount* mount = &vfs_mounts[vfs_mounts_len++];
    mount->path = path;
    mount->device = device;

    return vfs_mounts_len - 1;
}

enum error_code vfs_open(struct vfs_node* node, char* path)
{
    size_t offset;
    struct vfs_mount* mount = vfs_find_mount(path, &offset);
    struct vfs_device* device = vfs_get_device(mount->device);

    if (device == NULL)
        return ERROR;

    return device->fs->open(device, node, path + offset);
}

enum error_code vfs_close(struct vfs_node* node)
{
    struct vfs_mount* mount = vfs_find_mount(node->path, NULL);
    struct vfs_device* device = vfs_get_device(mount->device);

    if (device == NULL)
        return ERROR;

    return device->fs->close(device, node);
}

enum error_code vfs_read(struct vfs_node* node, uint8_t* buffer, uint32_t len)
{
    struct vfs_mount* mount = vfs_find_mount(node->path, NULL);
    struct vfs_device* device = vfs_get_device(mount->device);

    if (device == NULL)
        return ERROR;

    return device->fs->read(device, node, buffer, len);
}

enum error_code vfs_write(struct vfs_node* node, uint8_t* content, uint32_t len)
{
    struct vfs_mount* mount = vfs_find_mount(node->path, NULL);
    struct vfs_device* device = vfs_get_device(mount->device);

    if (device == NULL)
        return ERROR;

    return device->fs->write(device, node, content, len);
}

struct vfs_mount* vfs_find_mount(char* path, size_t* offset)
{
    char* copy = (char*) malloc(strlen(path) + 1);
    strcpy(copy, path);
    uint8_t index = strlen(path) - 1;

    while (index != (uint8_t) -1) {
        if (copy[index] == '/') {
            copy[index] = '\0';
            for (uint8_t n = 0; n < vfs_mounts_len; n++) {
                struct vfs_mount* mount = &vfs_mounts[n];
                if (strcmp(mount->path, copy) == 0) {
                    if (offset != NULL)
                        *offset = strlen(copy);
                    free((void*) copy);
                    return mount;
                }
            }
        }
        index--;
    }

    free((void*) copy);
    if (offset != NULL)
        *offset = 1;
    return &vfs_mounts[0];
}