#ifndef TAR_H
#define TAR_H

#include "vfs.h"

#include <stdint.h>

struct tar_header {
    uint8_t name[100];
    uint8_t mode[8];
    uint8_t owner_id[8];
    uint8_t group_id[8];
    uint8_t size[12];
    uint8_t modified[12];
    uint8_t checksum[8];
    uint8_t type;
    uint8_t linked[100];
};

struct vfs_fs tar_fs;

struct tar_node_priv {
    uint32_t address;
};

void tar_init(void);
enum error_code tar_open(struct vfs_device* device, struct vfs_node* node, char* path);
enum error_code tar_close(struct vfs_device* device, struct vfs_node* node);
enum error_code tar_read(struct vfs_device* device, struct vfs_node* node, uint8_t* buffer, uint32_t len);
enum error_code tar_write(struct vfs_device* device, struct vfs_node* node, uint8_t* content, uint32_t len);
uint32_t tar_parse_size(const uint8_t* num);
struct tar_header* tar_find_file(struct vfs_device* device, char* file, uint32_t* address_buffer);

#endif