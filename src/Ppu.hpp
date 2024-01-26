#pragma once

#include "PpuRegisters.hpp"

class Ppu
{
    public:
        Ppu() = default;

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

        void tick();

    private:
        PpuRegisters registers;
        unsigned openBusDecayTimer;
        u8 openBusContents;

        template <bool IsWrite>
        u8 access(u8 index, u8 data = 0);
};

#include "Ppu.inl"