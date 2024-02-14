#pragma once

#include <memory>
#include <array>

#include "Oam.hpp"
#include "Cartridge.hpp"
#include "PpuRegisters.hpp"
#include "PpuFramebuffer.hpp"

class Cpu;

class Ppu
{
    public:
        Ppu(const std::shared_ptr<Cpu>& cpu, 
            const std::shared_ptr<Cartridge>& cartridge,
            const std::shared_ptr<PpuFramebuffer>& framebuffer);

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

        void tick();

        bool isInVblank();

    private:
        std::weak_ptr<Cpu> cpuWeak;
        std::weak_ptr<Cartridge> cartridgeWeak;
        std::weak_ptr<PpuFramebuffer> framebufferWeak;
        PpuRegisters registers;

        unsigned openBusDecayTimer;
        u8 openBusContents;
        u8 vramReadBuffer;
        unsigned scanline;
        unsigned renderingPositionX;
        bool offsetToggleLatch;
        
        u16 patternTableAddress;
        u16 attributeTableAddress;
        
        u16 tilePattern;
        u16 tileAttributes;

        u32 bgShiftPattern;
        u32 bgShiftAttributes;

        Oam<64> oam;
        Oam<8> oam2;
        std::array<u8, 32> palette;

        void renderingTick();
        void renderPixel();

        void triggerNmi();
        void refreshOpenBus(u8 value);

        u8 ppuRead(u16 addr);
        void ppuWrite(u16 addr, u8 value);
};