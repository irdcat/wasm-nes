#pragma once

#include <memory>
#include <array>
#include <functional>

#include "OamData.hpp"
#include "Cartridge.hpp"
#include "PpuRegisters.hpp"
#include "Framebuffer.hpp"

class Ppu
{
    public:
        Ppu(const std::shared_ptr<Cartridge>& cartridge,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::function<void()>& nmiTriggerCallback,
            const std::function<void()>& vblankInterruptCallback);

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

        void tick();

        u8 getColorFromPalette(u8 palleteId, u8 pixel);

    private:
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<Framebuffer> framebuffer;
        PpuRegisters registers;

        unsigned openBusDecayTimer;
        u8 openBusContents;
        u8 vramReadBuffer;
        int scanline;
        unsigned scanlineEndPosition;
        unsigned renderingPositionX;
        bool offsetToggleLatch;
        bool evenOddFrameToggle;
        
        u16 patternTableAddress;
        u16 attributeTableAddress;
        
        u16 tilePattern;
        u16 tileAttributes;

        u32 bgShiftPattern;
        u32 bgShiftAttributes;

        std::array<u8, 256> oam;
        std::array<OamData, 8> oam2;
        std::array<OamData, 8> oam3;
        std::array<u8, 32> palette;

        u8 oamTempData;
        u8 spritePrimaryOamPosition;
        u8 spriteSecondaryOamPosition;
        u8 spriteRenderingPosition;

        std::function<void()> nmiTriggerCallback;
        std::function<void()> vblankInterruptCallback;

        u16 interleavePatternBytes(u8 lsb, u8 msb);

        void incrementScrollX();
        void incrementScrollY();

        void renderingTick();
        void spriteEvaluation();
        void renderPixel();

        void refreshOpenBus(u8 value);

        u8 ppuRead(u16 addr);
        void ppuWrite(u16 addr, u8 value);

        static constexpr const unsigned OPEN_BUS_DECAY_TICKS = 77777;
};