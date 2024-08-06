#pragma once

#include <memory>
#include <array>
#include <functional>

#include "OamData.hpp"
#include "Cartridge.hpp"
#include "PpuRegisters.hpp"

/**
 * PPU - Picture Processing Unit
 * This class represents 2C02 PPU (Used by NES in NTSC region)
 */
class Ppu
{
    public:
        static constexpr const unsigned SCREEN_WIDTH = 256;
        static constexpr const unsigned SCREEN_HEIGHT = 240;
        static constexpr const unsigned BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

        using Framebuffer = std::array<u8, BUFFER_SIZE>;

        Ppu(const std::shared_ptr<Cartridge>& cartridge,
            const std::function<void()>& nmiTriggerCallback,
            const std::function<void()>& vblankCallback);

        ~Ppu() = default;

        u8 read(u8 index);

        void write(u8 index, u8 data);

        void tick();

        const Framebuffer& getFramebuffer();

    private:
        std::shared_ptr<Cartridge> cartridge;
        std::array<u8, BUFFER_SIZE> framebuffer;

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
        u16 nametableAddress;
        
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
        std::function<void()> vblankCallback;

        u16 interleavePatternBytes(u8 lsb, u8 msb);

        void incrementScrollX();
        void incrementScrollY();

        void decodeTiles();
        void evaluateSprites();
        void renderPixel();

        void refreshOpenBus(u8 value);
        void decayOpenBus();

        u8 ppuRead(u16 addr);
        void ppuWrite(u16 addr, u8 value);

        static constexpr const unsigned OPEN_BUS_DECAY_TICKS = 77777;
};