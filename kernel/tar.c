#include "tar.h"
#include "liballoc.h"

#include <stdint.h>

unsigned int tar_parse_size(const uint8_t* num)
{
    unsigned int result = 0;
    unsigned int count = 1;

    for (unsigned int j = 11; j > 0; j--, count *= 8)
        result += ((num[j - 1] - '0') * count);
    
    return result;
}

unsigned int tar_count_headers(unsigned int address)
{
    unsigned int count = 0;

    for (;; count++) {
        struct tar_header* header = (struct tar_header*) address;

        if (header->name[0] == '\0')
            break;

        unsigned int size = tar_parse_size(header->size);
        address += ((size / 512) + 1) * 512;
        if (size % 512)
            address += 512;
    }

    return count;
}

struct tar_header** tar_parse_headers(unsigned int address, unsigned int count)
{
    struct tar_header** headers = (struct tar_header**) malloc(sizeof(unsigned int) * count);

    for (unsigned int index = 0; count > 0; count--, index++) {
        struct tar_header* header = (struct tar_header*) address;
        headers[index] = header;

        if (header->name[0] == '\0')
            break;

        unsigned int size = tar_parse_size(header->size);
        address += ((size / 512) + 1) * 512;
        if (size % 512)
            address += 512;
    }

    return headers;
}

uint8_t* tar_get_content(unsigned int address, struct tar_header* header)
{
    return (uint8_t*) (((unsigned int) header) + 512);
}