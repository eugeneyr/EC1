#include "RAM.h"
#include "CPU.h"
#include <memory>

static auto _ram_instance = std::make_unique<RAM>();

int32_t RAM::read_word(uint32_t address) {
    if (address >= RAM_SIZE_BYTES) {
        throw std::out_of_range("TRAP_MEMSIZE");
    }
    if (address % 4) {
        throw std::invalid_argument("TRAP_WORDBOUNDARY");
    }
    if (address < NO_OF_REGISTERS << 2u) {
        return CPU::get_instance().registers.words[address >> 2u];
    }
    return memory_bank.words[address >> 2u];
}

/**
 * Sets the value at the address. Returns the previous value at the address.
 */
int32_t RAM::write_word(uint32_t address, int32_t data) {
    if (address >= RAM_SIZE_BYTES) {
        throw std::out_of_range("TRAP_MEMSIZE");
    }
    if (address % 4) {
        throw std::invalid_argument("TRAP_WORDBOUNDARY");
    }
    int32_t old_value = 0;
    if (address < NO_OF_REGISTERS << 2u) {
        old_value = CPU::get_instance().registers.words[address >> 2u];
        CPU::get_instance().registers.words[address >> 2u] = data;
    } else {
        old_value = memory_bank.words[address >> 2u];
        memory_bank.words[address >> 2u] = data;
    }
    return old_value;
}

int8_t RAM::read_byte(uint32_t address) {
    if (address >= RAM_SIZE_BYTES) {
        throw std::out_of_range("TRAP_MEMSIZE");
    }
    if (address < NO_OF_REGISTERS) {
        return CPU::get_instance().registers.bytes[address];
    } else {
        return memory_bank.bytes[address];
    }
}

/**
 * Sets the value at the address. Returns the previous value at the address.
 */
int8_t RAM::write_byte(uint32_t address, int8_t data) {
    if (address >= RAM_SIZE_BYTES) {
        throw std::out_of_range("TRAP_MEMSIZE");
    }
    int8_t old_value = 0;
    if (address < NO_OF_REGISTERS) {
        old_value = CPU::get_instance().registers.bytes[address];
        CPU::get_instance().registers.bytes[address] = data;
    } else {
        old_value = memory_bank.bytes[address];
        memory_bank.bytes[address] = data;
    }
    return old_value;
}


RAM& RAM::get_instance() {
    return *_ram_instance;
}

int16_t RAM::read_halfword(uint32_t address) {
    if (address >= RAM_SIZE_BYTES) {
        throw std::out_of_range("TRAP_MEMSIZE");
    }
    if (address % 2) {
        throw std::invalid_argument("TRAP_HALFWORDBOUNDARY");
    }
    if (address < NO_OF_REGISTERS << 1u) {
        return CPU::get_instance().registers.halfwords[address >> 1u];
    }
    return memory_bank.halfwords[address >> 1u];
}

/**
 * Sets the value at the address. Returns the previous value at the address.
 */
int16_t RAM::write_halfword(uint32_t address, int16_t data) {
    if (address >= RAM_SIZE_BYTES) {
        throw std::out_of_range("TRAP_MEMSIZE");
    }
    if (address % 2) {
        throw std::invalid_argument("TRAP_HALFWORDBOUNDARY");
    }
    int16_t old_value = 0;
    if (address < NO_OF_REGISTERS << 1u) {
        old_value = CPU::get_instance().registers.halfwords[address >> 1u];
        CPU::get_instance().registers.halfwords[address >> 1u] = data;
    } else {
        old_value = memory_bank.halfwords[address >> 1u];
        memory_bank.halfwords[address >> 1u] = data;
    }
    return old_value;
}
