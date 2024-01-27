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