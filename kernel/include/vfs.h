#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

#define VFS_DEVICE_MAX 16
#define VFS_MOUNT_MAX 16

enum vfs_access_mode {
    VFS_READ,
    VFS_WRITE,
    VFS_READ_WRITE,
};

struct vfs_device {
    uint32_t id;
    char* name;
    struct vfs_fs* fs;
    uint8_t (*read)(uint8_t* buffer, uint32_t offset, uint32_t len);
    uint8_t (*write)(uint8_t* buffer, uint32_t offset, uint32_t len);
};

struct vfs_mount {
    uint32_t device;
    char* path;
};

struct vfs_node {
    char* path;
    uint32_t size;
    enum vfs_access_mode access_mode;
    uint32_t position;
    void* priv;
};

struct vfs_fs {
    char* name;
    uint8_t (*open)(struct vfs_device* device, struct vfs_node* node, char* path);
    uint8_t (*close)(struct vfs_device* device, struct vfs_node* node);
    uint8_t (*read)(struct vfs_device* device, struct vfs_node* node, uint8_t* buffer, uint32_t len);
    uint8_t (*write)(struct vfs_device* device, struct vfs_node* node, uint8_t* content, uint32_t len);
};

struct vfs_device vfs_devices[VFS_DEVICE_MAX];
struct vfs_mount vfs_mounts[VFS_MOUNT_MAX];
uint8_t vfs_devices_len;
uint8_t vfs_mounts_len;
uint32_t vfs_device_next_id;

inline uint32_t vfs_generate_device_id(void)
{
    return vfs_device_next_id++;
}

inline struct vfs_device* vfs_get_device(uint32_t id)
{
    for (uint32_t index = 0; index < VFS_DEVICE_MAX; index++) {
        struct vfs_device* device = &vfs_devices[index];
        if (device->id == id)
            return device;
    }
    return NULL;
}

void vfs_init(void);
uint32_t vfs_register_device(
    char* name,
    struct vfs_fs* fs,
    uint8_t (*read)(uint8_t* buffer, uint32_t offset, uint32_t len),
    uint8_t (*write)(uint8_t* buffer, uint32_t offset, uint32_t len));
uint32_t vfs_mount(char* path, uint32_t device);
uint8_t vfs_open(struct vfs_node* buffer, char* path);
uint8_t vfs_close(struct vfs_node* node);
uint8_t vfs_read(struct vfs_node* node, uint8_t* buffer, uint32_t len);
uint8_t vfs_write(struct vfs_node* node, uint8_t* content, uint32_t len);
uint8_t vfs_find_mount(char* path, uint8_t* offset);

#endif