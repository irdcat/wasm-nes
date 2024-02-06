#pragma once

#include "RegisterBit.hpp"

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