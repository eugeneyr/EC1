cmake_minimum_required(VERSION 3.10)
project(EC1)

set(CMAKE_CXX_STANDARD 17)

add_executable(EC1 main.cpp Instruction.cpp Instruction.h AddressSpace.cpp AddressSpace.h ALU.cpp ALU.h Supervisor.cpp Supervisor.h CPU.cpp CPU.h RAM.cpp RAM.h constants.h FPU.cpp FPU.h)