#include <unordered_map>
#include <iostream>

#include "Instruction.h"

// G, L, and E bits of the CCR register
const uint8_t CCR_E_BIT = 0x01;
const uint8_t CCR_G_BIT = 0x02;
const uint8_t CCR_L_BIT = 0x04;


// Opcode constants:

// Load Register
const uint8_t OPCODE_LR = 0x00;
// Load
const uint8_t OPCODE_L = 0x20;
// Load Immediate
const uint8_t OPCODE_LI = 0x40;
// Load Character
const uint8_t OPCODE_LC = 0x60;

// Load Negative Register
const uint8_t OPCODE_LNR = 0x01;
// Load Negative
const uint8_t OPCODE_LN = 0x21;
// Load Negative Immediate
const uint8_t OPCODE_LNI = 0x41;
// Load Negative Character
const uint8_t OPCODE_LNC = 0x61;

// end of the opcode constants section

// First register / operand mask in the instruction half word
const uint16_t R1_MASK = 0x0F00;
const uint16_t R1_OFFSET = 0x08;

// Second register / operand mask in the instruction half word
const uint16_t R2_MASK = 0xF000;
const uint16_t R2_OFFSET = 0x0C;

inline uint16_t get_r1(uint16_t arg) {
    return (arg & R1_MASK) >> R1_OFFSET;
}

inline uint16_t get_r2(uint16_t arg) {
    return (arg & R2_MASK) >> R2_OFFSET;
}


class LoadRegister : public Instruction {
public:
    void execute(uint16_t inst_start, uint16_t inst_end, CPU &cpu, RAM &ram) const override {
        std::cout << "LR" << std::endl;
        auto indirect = inst_start & 0x01;
        auto r1 = get_r1(inst_start);
        auto r2 = get_r2(inst_start);
        auto value = ram.read_word(r2 << 0x02);
        if (indirect) {
            value = ram.read_word(value);
        }
        ram.write_word(r1 << 0x02, value);
        if (!value) {
            cpu.ccr = CCR_E_BIT;
        } else if (value > 0) {
            cpu.ccr = CCR_G_BIT;
        } else {
            cpu.ccr = CCR_L_BIT;
        }

        cpu.ilc += 2;
    }

    bool is_full_word() const override {
        return false;
    }

    LoadRegister() = default;

    virtual ~LoadRegister() = default;
};


class RegisterAndStorageInstruction: public Instruction {
public:
    bool is_full_word() const override {
        return true;
    }

protected:
    virtual uint32_t get_effective_address(uint16_t inst_start, uint16_t address_field, CPU &cpu, RAM &ram) const {
        auto indirect = inst_start & 0x01;
        auto r2 = get_r2(inst_start);;
        int32_t rv = address_field;
        if (indirect) {
            rv = ram.read_word((uint32_t)rv);
        }
        if (r2) {
            rv += ram.read_word(r2 << 0x02);
        }
        return (uint32_t)rv;
    }

};

class ImmediateInstruction: public Instruction {
public:
    bool is_full_word() const override {
        return true;
    }

protected:
    virtual int32_t get_immediate_value(uint16_t inst_start, uint16_t address_field, CPU &cpu, RAM &ram) const {
        return (int32_t)(((int16_t)(inst_start & 0xF000) >> 0x04) | address_field);
    }
};


class Load : public RegisterAndStorageInstruction {
public:
    void execute(uint16_t inst_start, uint16_t inst_end, CPU &cpu, RAM &ram) const override {
        std::cout << "L" << std::endl;
        auto indirect = inst_start & 0x01;
        auto r1 = get_r1(inst_start);
        auto effective_address = get_effective_address(inst_start, inst_end, cpu, ram);
        auto value = ram.read_word(effective_address);

        ram.write_word(r1 << 0x02, value);
        if (!value) {
            cpu.ccr = CCR_E_BIT;
        } else if (value > 0) {
            cpu.ccr = CCR_G_BIT;
        } else {
            cpu.ccr = CCR_L_BIT;
        }

        cpu.ilc += 4;
    }

    Load() = default;

    virtual ~Load() = default;
};

class LoadImmediate : public ImmediateInstruction {
public:
    void execute(uint16_t inst_start, uint16_t inst_end, CPU &cpu, RAM &ram) const override {
        std::cout << "L" << std::endl;
        auto r1 = get_r1(inst_start);
        auto immediate_value = get_immediate_value(inst_start, inst_end, cpu, ram);

        ram.write_word(r1 << 0x02, immediate_value);
        if (!immediate_value) {
            cpu.ccr = CCR_E_BIT;
        } else if (immediate_value > 0) {
            cpu.ccr = CCR_G_BIT;
        } else {
            cpu.ccr = CCR_L_BIT;
        }

        cpu.ilc += 4;
    }

    LoadImmediate() = default;

    virtual ~LoadImmediate() = default;
};


class CharacterInstruction: public RegisterAndStorageInstruction {
};


class LoadCharacter : public CharacterInstruction {
public:
    void execute(uint16_t inst_start, uint16_t inst_end, CPU &cpu, RAM &ram) const override {
        std::cout << "L" << std::endl;
        auto indirect = inst_start & 0x01;
        auto r1 = get_r1(inst_start);
        auto effective_address = get_effective_address(inst_start, inst_end, cpu, ram);
        auto value = ram.read_byte(effective_address);

        ram.write_word(r1 << 0x02, value);
        if (!value) {
            cpu.ccr = CCR_E_BIT;
        } else {
            cpu.ccr = CCR_G_BIT;
        }

        cpu.ilc += 4;
    }

    LoadCharacter() = default;

    virtual ~LoadCharacter() = default;
};



Instruction &Instruction::get_instruction(uint16_t inst_start, uint16_t inst_end) {
    static std::unordered_map<uint8_t, std::unique_ptr<Instruction>> opcodes_to_instructions;
    static bool loaded;
    if (!loaded) {
        opcodes_to_instructions[OPCODE_LR] = std::make_unique<LoadRegister>();
        opcodes_to_instructions[OPCODE_L] = std::make_unique<Load>();
        opcodes_to_instructions[OPCODE_LI] = std::make_unique<LoadImmediate>();
        opcodes_to_instructions[OPCODE_LC] = std::make_unique<LoadCharacter>();
        loaded = true;
    }
    auto opcode = (uint8_t) (inst_start > 1);
    return *opcodes_to_instructions.at(opcode);
}

bool Instruction::is_full_word(uint16_t instruction_start) {
    return get_instruction(instruction_start).is_full_word();
}

Instruction::Instruction() = default;

Instruction::~Instruction() = default;


