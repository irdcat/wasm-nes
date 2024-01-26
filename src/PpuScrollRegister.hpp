#pragma once

#include "Types.hpp"

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