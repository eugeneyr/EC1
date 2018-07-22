#include <unordered_map>
#include <iostream>

#include "Instruction.h"

// Load Register
const uint8_t OPCODE_LR = 0x00;
// Load
const uint8_t OPCODE_L = 0x20;
// Load Immediate
const uint8_t OPCODE_IM = 0x40;
// Load Character
const uint8_t OPCODE_CH = 0x60;
// Load Negative Register
const uint8_t OPCODE_LNR = 0x01;
// Load Negative
const uint8_t OPCODE_LN = 0x21;
// Load Negative Immediate
const uint8_t OPCODE_LNI = 0x41;

class LoadRegister : public Instruction {
public:
    void execute(CPU &cpu, RAM &ram) const override {
        std::cout << "LR" << std::endl;
        cpu.ilc += 2;
    }

    bool is_full_word() const override {
        return false;
    }

    LoadRegister() {
    }

    virtual ~LoadRegister() {
    }
};


Instruction &Instruction::get_instruction(uint16_t inst_start, uint16_t inst_end) {
    static std::unordered_map<uint8_t, std::unique_ptr<Instruction>> opcodes_to_instructions;
    static bool loaded;
    if (!loaded) {
        opcodes_to_instructions[OPCODE_LR] = std::make_unique<LoadRegister>();

        loaded = true;
    }
    auto opcode = (uint8_t) (inst_start > 1);
    return *opcodes_to_instructions.at(opcode);
}

bool Instruction::is_full_word(uint16_t instruction_start) {
    return get_instruction(instruction_start).is_full_word();
}

Instruction::Instruction() {}

Instruction::~Instruction() {

}


