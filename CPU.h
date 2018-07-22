#ifndef EC1_CPU_H
#define EC1_CPU_H

#include "constants.h"

class CPU {
protected:
    bool halt;

public:
    uint8_t ccr;
    uint32_t ilc;

    union {
        int32_t words[NO_OF_REGISTERS];
        int16_t halfwords[NO_OF_REGISTERS << 1u];
        int8_t bytes[NO_OF_REGISTERS << 2u];
    } registers;

    void step();

    void main_loop();

    static CPU& get_instance();

    bool isHalt() const;

    void setHalt(bool halt);

    CPU(bool halt, uint8_t ccr, uint32_t ilc);

    CPU();

    virtual ~CPU();
};


#endif //EC1_CPU_H
