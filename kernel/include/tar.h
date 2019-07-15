#ifndef TAR_H
#define TAR_H

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

unsigned int tar_parse_size(const uint8_t* num);
unsigned int tar_count_headers(unsigned int address);
struct tar_header** tar_parse_headers(unsigned int address, unsigned int count);
uint8_t* tar_get_content(unsigned int address, struct tar_header* header);

#endif