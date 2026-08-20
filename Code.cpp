#include "Code.h"

Code::Code() : top(0) {
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        bits[i] = 0;
    }
}

bool Code::push_bit(uint8_t bit) {
    if (top >= ALPHABET) return false;
    if (bit) {
        bits[top / 8] |= (1 << (top % 8));
    } else {
        bits[top / 8] &= ~(1 << (top % 8));
    }
    top++;
    return true;
}

bool Code::pop_bit(uint8_t &bit) {
    if (top == 0) return false;
    top--;
    bit = (bits[top / 8] & (1 << (top % 8))) ? 1 : 0;
    return true;
}

bool Code::is_empty() const {
    return top == 0;
}

uint32_t Code::size() const {
    return top;
}