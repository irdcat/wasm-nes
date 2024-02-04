#pragma once

#include "FlagRegister.hpp"

struct CpuRegisters
{
    u8 a;           // Accumulator
    u8 x;           // Index X
    u8 y;           // Index Y
    u16 pc;         // Program Counter
    u8 s;           // Stack Pointer
    FlagRegister p; // Processor Status
};