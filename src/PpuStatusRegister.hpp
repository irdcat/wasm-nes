#pragma once

#include "Types.hpp"

union PpuStatusRegister
{
    u8 raw;
    struct 
    {
        u8 : 5;
        u8 spriteOverflow : 1;
        u8 spriteZeroHit : 1;
        u8 vblankStarted : 1;
    };

    operator u8&() { return raw; }

    PpuStatusRegister& operator=(u8 value) { raw = value; return *this; }
};