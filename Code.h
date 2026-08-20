#ifndef CODE_H
#define CODE_H

#include "defines.h"

class Code {
public:
    uint32_t top;
    uint8_t bits[MAX_CODE_SIZE];

    Code();
    bool push_bit(uint8_t bit);
    bool pop_bit(uint8_t &bit);
    bool is_empty() const;
    uint32_t size() const;
};

#endif