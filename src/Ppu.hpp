#pragma once

#include <memory>
#include <array>
#include <functional>

#include "Oam.hpp"
#include "Cartridge.hpp"
#include "PpuRegisters.hpp"
#include "PpuFramebuffer.hpp"

class Ppu
{
    public:
        Ppu(const std::shared_ptr<Cartridge>& cartridge,
            const std::shared_ptr<PpuFramebuffer>& framebuffer,
            const std::function<void()>& nmiTriggerCallback);

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

        void tick();

        bool isInVblank() const;

    private:
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<PpuFramebuffer> framebuffer;
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

        std::function<void()> nmiTriggerCallback;

        void renderingTick();
        void renderPixel();

        void triggerNmi();
        void refreshOpenBus(u8 value);

        u8 ppuRead(u16 addr);
        void ppuWrite(u16 addr, u8 value);
};