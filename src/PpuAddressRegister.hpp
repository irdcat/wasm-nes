#pragma once

#include "Types.hpp"

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