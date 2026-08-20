#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>

#define ALPHABET 256
#define MAGIC 0xDEADEAEF
#define MAX_CODE_SIZE (ALPHABET / 8)
#define BLOCK 4096

struct Header {
    uint32_t signature;
    uint32_t tree_size;
    uint64_t file_size;
};

#endif