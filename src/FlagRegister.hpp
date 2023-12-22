#pragma once

#include "Types.hpp"

union FlagRegister
{
    u8 raw;
    struct
    {
        u8 carry : 1;
        u8 zero : 1;
        u8 interruptDisable : 1;
        u8 decimal : 1;
        u8 breakFlag : 1;
        u8 : 1;
        u8 overflow : 1;
        u8 negative : 1;
    };

    operator u8&() { return raw; }

    FlagRegister& operator=(u8 value) { raw = value; return *this; }
};