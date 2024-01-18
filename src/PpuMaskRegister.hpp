#pragma once

#include "Types.hpp"

union PpuMaskRegister
{
    u8 raw;
    struct
    {
        u8 greyscale : 1;
        u8 showBg8 : 1;
        u8 showSp8 : 1;
        u8 showBg : 1;
        u8 showSp : 1;
        u8 emphasizeRed : 1;
        u8 emphasizeGreen : 1;
        u8 emphasizeBlue : 1;
    };

    operator u8&() { return raw; }

    PpuMaskRegister& operator=(u8 value) { raw = value; return *this; }
};