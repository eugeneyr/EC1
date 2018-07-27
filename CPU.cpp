#include <memory>
#include "constants.h"
#include "CPU.h"
#include "RAM.h"
#include "Instruction.h"

static auto _cpu_instance = std::make_unique<CPU>();

CPU& CPU::get_instance() {
    return *_cpu_instance;
}

void CPU::step() {
    decltype(auto) ram = RAM::get_instance();
    decltype(auto) instruction_start = (uint16_t) (ram.read_halfword(ilc++));
    auto instruction_end = (uint16_t) (Instruction::is_full_word(instruction_start) ? ram.read_halfword(ilc) : 0);
    decltype(auto) instruction = Instruction::get_instruction(instruction_start, instruction_end);
    instruction.execute(instruction_start, instruction_end, *this, ram);
}

void CPU::main_loop() {
    // TODO use ticks and time the loop
    while (!halt) {
        step();
    }
}

inline bool CPU::isHalt() const {
    return halt;
}

inline void CPU::setHalt(bool halt) {
    CPU::halt = halt;
}

CPU::CPU(bool halt, uint8_t ccr, uint32_t ilc) : halt(halt), ccr(ccr), ilc(ilc) {

}

CPU::CPU() : halt(false), ccr(0), ilc(NO_OF_REGISTERS << 4) {
}

CPU::~CPU() {

}
