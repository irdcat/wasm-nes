#pragma once

#include <memory>
#include <array>

#include "Oam.hpp"
#include "Cartridge.hpp"
#include "PpuRegisters.hpp"

class Cpu;

class Ppu
{
    public:
        Ppu(const std::shared_ptr<Cpu>& cpu, const std::shared_ptr<Cartridge>& cartridge);

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

        void tick();

    private:
        std::weak_ptr<Cpu> cpuWeak;
        std::weak_ptr<Cartridge> cartridgeWeak;
        PpuRegisters registers;

        unsigned openBusDecayTimer;
        u8 openBusContents;
        u8 vramReadBuffer;
        int scanline;
        unsigned renderingPositionX;

        Oam<64> oam;
        Oam<8> oam2;
        std::array<u8, 32> palette;

        template <bool IsWrite>
        u8 access(u8 index, u8 data = 0);

        void triggerNmi();

        u8 ppuRead(u16 addr);
        void ppuWrite(u16 addr, u8 value);
};

#include "Ppu.inl"