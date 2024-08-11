#pragma once

#include "RegisterBit.hpp"

union PpuCtrlRegister
{
    u8 raw;

    // Base nametable select
    // Lower bit selects horitonzal nametable
    // Higher bit selects vertical nametable 
    RegisterBit<0, 2> baseNametableAddress;
    // VRam Address Increment flag
    // 0 - VRam address incremented by 1
    // 1 - VRam address incremented by 32
    RegisterBit<2> vramAddressIncrement;
    // Pattern table used for rendering sprites
    // 0 - 0x0000 - 0x0FFF
    // 1 - 0x1000 - 0x1FFF
    // Note that this bit is ignored when dealing with 8x16 sprites,
    // and first pattern table 0 is always selected
    RegisterBit<3> spritePatternTableAddress;
    // Pattern table used for rendering background
    // 0 - 0x0000 - 0x0FFF
    // 1 - 0x1000 - 0x1FFF
    RegisterBit<4> backgroundPatternTableAddress;
    // Select sprite size
    // 0 - 8x8
    // 1 - 8x16
    RegisterBit<5> spriteSize;
    // Master/slave select
    // Unused during emulation
    RegisterBit<6> masterSlaveSelect;
    // VBlank NMI
    // 0 - Non-Maskable Interrupt upon entering VBlank period is disabled
    // 1 - Non-Maskable Interrupt upon entering VBlank period is enabled
    RegisterBit<7> VBlankNmi;

    operator u8&() { return raw; }

    PpuCtrlRegister& operator=(u8 value) { raw = value; return *this; }
};

union PpuMaskRegister
{
    u8 raw;
    // 0 - Normal color
    // 1 - Produce a grayscale display
    RegisterBit<0> greyscale;
    // 0 - Hide background in 8 leftmost pixels on the screen
    // 1 - Show
    RegisterBit<1> showBg8;
    // 0 - Hide sprites in 8 leftmost pixels on the screen
    // 1 - Show
    RegisterBit<2> showSp8;
    // Show background
    RegisterBit<3> showBg;
    // Show sprites
    RegisterBit<4> showSp;
    // Emphasize red color
    RegisterBit<5> emphasizeRed;
    // Emphasize green color
    RegisterBit<6> emphasizeGreen;
    // Emphasize blue color
    RegisterBit<7> emphasizeBlue;

    // Combination of ShowBG and ShowSP
    // Returns non-zero result when either sprites, background or both are enabled.
    RegisterBit<3, 2> showBgSp;

    operator u8&() { return raw; }

    PpuMaskRegister& operator=(u8 value) { raw = value; return *this; }
};

union PpuStatusRegister
{
    u8 raw;
    // Sprite overflow. The intent was for this flag to be set
    // whenever more than eight sprites appear on a scanline, but a
    // hardware bug causes the actual behavior to be more complicated
    // and generate false positives as well as false negatives
    RegisterBit<5> spriteOverflow;
    // Sprite 0 Hit. Set when non-zero pixel of a sprite overlaps 
    // or is overlapped by a non-zero background pixel.
    RegisterBit<6> spriteZeroHit;
    // In VBlank. Set when PPU enters VBlank period.
    RegisterBit<7> inVBlank;

    operator u8&() { return raw; }

    PpuStatusRegister& operator=(u8 value) { raw = value; return *this; }
};

union OamAddressRegister
{
    u8 raw;
    // Index of a byte of the data associated with the sprite.
    RegisterBit<0, 2> spriteDataIndex;
    // Index of the sprite.
    RegisterBit<2, 6> spriteIndex;

    operator u8&() { return raw; }

    OamAddressRegister& operator=(u8 value) { raw = value; return *this; }
};

/**
 * Internal Address register.
 * PPU uses two registers with roughly this structure:
 * - Temporary VRAM Address Register, also called T or taddr
 * - Current VRAM Address Register, also called V or vaddr
 * 
 * When rendering is disabled Current VRAM Address Register is used, 
 * just as a 15 bit address.
 * 
 * When rendering is enabled Current VRAM Address Register is used
 * as follows:
 * yyyNNYYYYYXXXXX
 * 
 * where
 * y - Fine Y
 * N - Nametable select
 * Y - Coarse Y
 * X - Coarse X
 */
union PpuInternalRegister
{
    // Bits 15-17 are occupied by fineX, which on a real PPU
    // is a separate 3 bit register, but it is included here for convenience.
    // Thus V and T registers are 15 bits long, thus this is the amount of bits
    // that are associated with raw value.
    RegisterBit<0, 15, u32> raw;
    // Coarse X coordinate of the scroll
    RegisterBit<0, 5, u32> coarseX;
    // Coarse Y coordinate of the scroll
    RegisterBit<5, 5, u32> coarseY;
    // Base nametable select. This value contains 2 bit index of nametable.
    RegisterBit<10, 2, u32> baseNametable;
    // Base horizontal nametable select
    RegisterBit<10, 1, u32> baseHorizontalNametable;
    // Base vertical nametable select
    RegisterBit<11, 1, u32> baseVerticalNametable;
    // Fine Y coordinate of the scroll
    RegisterBit<12, 3, u32> fineY;
    // Fine X coordinate of the scroll
    RegisterBit<15, 3, u32> fineX;
    // Least significant bits of VRAM Address
    RegisterBit<0, 8, u32> vramAddressLow;
    // Most significant bits of VRAM Address
    RegisterBit<8, 7, u32> vramAddressHigh;
    // VRAM address
    RegisterBit<0, 15, u32> vramAddress;

    operator u16() { return raw; }

    PpuInternalRegister& operator=(u16 value) { raw = value; return *this; }
};

struct PpuRegisters
{
    PpuCtrlRegister ppuCtrl;
    PpuMaskRegister ppuMask;
    PpuStatusRegister ppuStatus;
    OamAddressRegister oamAddr;
    PpuInternalRegister vaddr;
    PpuInternalRegister taddr;
};