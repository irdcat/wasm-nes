#pragma once

#include "RegisterBit.hpp"

union PpuStatusRegister
{
    u8 raw;
    RegisterBit<5> spriteOverflow;
    RegisterBit<6> spriteZeroHit;
    RegisterBit<7> inVBlank;

    operator u8&() { return raw; }

    PpuStatusRegister& operator=(u8 value) { raw = value; return *this; }
};