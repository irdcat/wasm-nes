#pragma once

#include "Types.hpp"

union PpuCtrlRegister
{
    u8 raw;
    struct 
    {
        u8 baseNametableAddress : 2;
        u8 vramAddressIncrement : 1;
        u8 spritePatternTableAddress : 1;
        u8 backgroundPatternTableAddress : 1;
        u8 spriteSize : 1;
        u8 masterSlaveSelect : 1;
        u8 vblankNmi : 1;
    };

    operator u8&() { return raw; }

    PpuCtrlRegister& operator=(u8 value) { raw = value; return *this; }
};