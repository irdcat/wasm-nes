#pragma once

#include "RegisterBit.hpp"

union FlagRegister
{
    u8 raw;
    RegisterBit<0> carry;
    RegisterBit<1> zero;
    RegisterBit<2> interruptDisable;
    RegisterBit<3> decimal;
    RegisterBit<4> breakFlag;
    RegisterBit<6> overflow;
    RegisterBit<7> negative;

    operator u8&() { return raw; }

    FlagRegister& operator=(u8 value) { raw = value; return *this; }
};

struct CpuRegisters
{
    u8 a;           // Accumulator
    u8 x;           // Index X
    u8 y;           // Index Y
    u16 pc;         // Program Counter
    u8 s;           // Stack Pointer
    FlagRegister p; // Processor Status
};