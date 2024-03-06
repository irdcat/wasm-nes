#pragma once

#include "RegisterBit.hpp"

union PpuCtrlRegister
{
    u8 raw;
    RegisterBit<0, 2> baseNametableAddress;
    RegisterBit<2> vramAddressIncrement;
    RegisterBit<3> spritePatternTableAddress;
    RegisterBit<4> backgroundPatternTableAddress;
    RegisterBit<5> spriteSize;
    RegisterBit<6> masterSlaveSelect;
    RegisterBit<7> VBlankNmi;

    operator u8&() { return raw; }

    PpuCtrlRegister& operator=(u8 value) { raw = value; return *this; }
};

union PpuMaskRegister
{
    u8 raw;
    RegisterBit<0> greyscale;
    RegisterBit<1> showBg8;
    RegisterBit<2> showSp8;
    RegisterBit<3> showBg;
    RegisterBit<4> showSp;
    RegisterBit<5> emphasizeRed;
    RegisterBit<6> emphasizeGreen;
    RegisterBit<7> emphasizeBlue;

    RegisterBit<3, 2> showBgSp;

    operator u8&() { return raw; }

    PpuMaskRegister& operator=(u8 value) { raw = value; return *this; }
};

union PpuStatusRegister
{
    u8 raw;
    RegisterBit<5> spriteOverflow;
    RegisterBit<6> spriteZeroHit;
    RegisterBit<7> inVBlank;

    operator u8&() { return raw; }

    PpuStatusRegister& operator=(u8 value) { raw = value; return *this; }
};

union PpuInternalRegister
{
    RegisterBit<0, 15, u32> raw;
    RegisterBit<0, 5, u32> coarseX;
    RegisterBit<5, 5, u32> coarseY;
    RegisterBit<10, 2, u32> baseNametable;
    RegisterBit<10, 1, u32> baseHorizontalNametable;
    RegisterBit<11, 1, u32> baseVerticalNametable;
    RegisterBit<12, 3, u32> fineY;
    RegisterBit<15, 3, u32> fineX;
    RegisterBit<0, 8, u32> vramAddressLow;
    RegisterBit<8, 7, u32> vramAddressHigh;
    RegisterBit<0, 14, u32> vramAddress;

    operator u16() { return raw; }

    PpuInternalRegister& operator=(u16 value) { raw = value; return *this; }
};

struct PpuRegisters
{
    PpuCtrlRegister ppuCtrl;
    PpuMaskRegister ppuMask;
    PpuStatusRegister ppuStatus;
    u8 oamAddr;
    PpuInternalRegister currentVramAddress;
    PpuInternalRegister temporaryVramAddress;
};