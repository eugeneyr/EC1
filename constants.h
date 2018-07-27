#ifndef EC1_CONSTANTS_H
#define EC1_CONSTANTS_H

#include <cinttypes>

const uint32_t RAM_SIZE_WORDS = 0x4000;
const uint32_t RAM_SIZE_HALFWORDS = RAM_SIZE_WORDS << 1;
const uint32_t RAM_SIZE_BYTES = RAM_SIZE_WORDS << 2;
const uint32_t NO_OF_REGISTERS = 0x10;

#endif //EC1_CONSTANTS_H
