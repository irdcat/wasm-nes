#pragma once

#include <memory>

#include "Oam.hpp"
#include "PpuRegisters.hpp"

class Cpu;

class Ppu
{
    public:
        Ppu(const std::shared_ptr<Cpu>& cpu);

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

        void tick();

    private:
        std::weak_ptr<Cpu> cpuWeak;
        PpuRegisters registers;

        unsigned openBusDecayTimer;
        u8 openBusContents;
        u8 vramReadBuffer;
        int scanline;
        unsigned renderingPositionX;

        Oam<64> oam;
        Oam<8> oam2;

        template <bool IsWrite>
        u8 access(u8 index, u8 data = 0);

        void triggerNmi();
};

#include "Ppu.inl"