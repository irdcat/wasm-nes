#pragma once

#include "PpuRegisters.hpp"

class Ppu
{
    public:
        Ppu() = default;

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

    private:
        PpuRegisters ppuRegisters;
};