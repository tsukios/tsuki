#include "tar.h"
#include "liballoc.h"
#include "log.h"
#include "vfs.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

void tar_init(void)
{
    tar_fs.name = "tar";
    tar_fs.open = &tar_open;
    tar_fs.close = &tar_close;
    tar_fs.read = &tar_read;
    tar_fs.write = &tar_write;

    log(LOG_OK, "Tar module initialized\n");
}

uint8_t tar_open(struct vfs_device* device, struct vfs_node* node, char* path)
{
    uint32_t address;
    struct tar_header* header = tar_find_file(device, path, &address);

    if (header == NULL)
        return 1;
    
    node->path = path;
    node->size = tar_parse_size(header->size);
    node->access_mode = VFS_READ;
    node->position = 0;

    struct tar_node_priv* priv = (struct tar_node_priv*) malloc(sizeof(struct tar_node_priv));
    priv->address = address;
    node->priv = (void*) priv;

    return 0;
}

uint8_t tar_close(struct vfs_device* device, struct vfs_node* node)
{
    free(node->priv);
    free((void*) node);

    return 0;
}

uint8_t tar_read(struct vfs_device* device, struct vfs_node* node, uint8_t* buffer, uint32_t len)
{
    uint32_t address = ((struct tar_node_priv*) node->priv)->address;
    address += 512;

    // First case: position is already beyond EOF
    if (node->position >= node->size) {
        // Don't read anything
        return 2; // EOF
    // Second case: new position is beyond EOF
    } else if (node->position + len > node->size) {
        // Adjust len to read up to EOF only
        len = node->size - node->position;
        device->read(buffer, address + node->position, len);
        node->position += len;
        return 2; // EOF
    // Third case: new position is within file
    } else {
        // Do nothing, just read
        device->read(buffer, address + node->position, len);
        node->position += len;
        return 0;
    }
}

uint8_t tar_write(struct vfs_device* device, struct vfs_node* node, uint8_t* content, uint32_t len)
{
    // tar is read-only
    return 1;
}

uint32_t tar_parse_size(const uint8_t* num)
{
    uint32_t result = 0;
    uint32_t count = 1;

    for (uint32_t j = 11; j > 0; j--, count *= 8)
        result += ((num[j - 1] - '0') * count);
    
    return result;
}

struct tar_header* tar_find_file(struct vfs_device* device, char* file, uint32_t* address_buffer)
{
    uint32_t address = 0;

    while (1) {
        uint8_t buffer[sizeof(struct tar_header)];
        if (device->read(buffer, address, sizeof(buffer)) == 1)
            return NULL;

        struct tar_header* header = (struct tar_header*) buffer;

        if (header->name[0] == '\0')
            return NULL;
        
        log(LOG_OK, "%s\n", header->name);

        if (strcmp((char*) header->name, file) == 0) {
            if (address_buffer != NULL)
                *address_buffer = address;
            return header;
        }

        uint32_t size = tar_parse_size(header->size);
        address += ((size / 512) + 1) * 512;
        if (size % 512)
            address += 512;
    }
}