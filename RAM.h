#ifndef EC1_RAM_H
#define EC1_RAM_H

#include "constants.h"
#include <stdexcept>


typedef union {
    int32_t words[RAM_SIZE_WORDS];
    int8_t bytes[RAM_SIZE_BYTES];
    int16_t halfwords[RAM_SIZE_HALFWORDS];
} MEMORY_BANK;

class RAM {
protected:
    MEMORY_BANK memory_bank;
public:
    int32_t read_word(uint32_t address);

    int32_t write_word(uint32_t address, int32_t data);

    int8_t read_byte(uint32_t address);

    int8_t write_byte(uint32_t address, int8_t data);

    int16_t read_halfword(uint32_t address);

    int16_t write_halfword(uint32_t address, int16_t data);

    static RAM& get_instance();
};


#endif //EC1_RAM_H
