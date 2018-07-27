#ifndef EC1_INSTRUCTION_H
#define EC1_INSTRUCTION_H

#include <cstdint>
#include "RAM.h"
#include "CPU.h"

class Instruction {
public:
    virtual void execute(uint16_t inst_start, uint16_t inst_end, CPU& cpu, RAM& ram) const = 0;
    virtual bool is_full_word() const = 0;

    static Instruction& get_instruction(uint16_t inst_start, uint16_t inst_end = 0u);
    static bool is_full_word(uint16_t instruction_start);

    Instruction();

    virtual ~Instruction();
};

#endif //EC1_INSTRUCTION_H
