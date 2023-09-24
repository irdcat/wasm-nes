#pragma once

#include "FlagRegister.hpp"

class CpuRegisters
{
    public:
        CpuRegisters() = default;

        ~CpuRegisters() = default;

        u8& getA() { return a; }

        u8& getX() { return x; }

        u8& getY() { return y; }

        u16& getPc() { return pc; }

        u8& getS() { return s; }

        FlagRegister& getP() { return p; }

    private:
        u8 a;           // Accumulator
        u8 x;           // Index X
        u8 y;           // Index Y
        u16 pc;         // Program Counter
        u8 s;           // Stack Pointer
        FlagRegister p; // Processor Status
};