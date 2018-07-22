#include <iostream>
#include "CPU.h"

int main(int argc, const char ** argv) {
    std::cout << "EC-1 Simulator v.0.1" << std::endl;
    decltype(auto) cpu = CPU::get_instance();
    cpu.step();
    return 0;
}