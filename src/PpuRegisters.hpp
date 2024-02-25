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

union PpuScrollRegister
{
    u16 raw;
    struct
    {
        u8 x;
        u8 y;
    };

    operator u16&() { return raw; }

    PpuScrollRegister& operator=(u16 value) { raw = value; return *this; }
};

union PpuAddressRegister
{
    u16 raw;
    struct 
    {
        u8 low;
        u8 high;
    };
    
    operator u16&() { return raw; }

    PpuAddressRegister& operator=(u16 value) { raw = value; return *this; }
};

struct PpuRegisters
{
    PpuCtrlRegister ppuCtrl;
    PpuMaskRegister ppuMask;
    PpuStatusRegister ppuStatus;
    u8 oamAddr;
    PpuScrollRegister ppuScroll;
    PpuAddressRegister ppuAddr;
};