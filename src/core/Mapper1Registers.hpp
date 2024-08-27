#pragma once

#include "RegisterBit.hpp"

union Mapper1ControlRegister
{
    RegisterBit<0, 2> mirroring;
    RegisterBit<2, 2> prgRomBankMode;
    RegisterBit<4> chrRomBankMode;
    u8 raw;

    operator u8() { return raw; }

    Mapper1ControlRegister& operator=(u8 value) { raw = value; return *this; }
};

union Mapper1PrgBankRegister
{
    RegisterBit<0, 4> bank;
    RegisterBit<4> ramEnable;
    u8 raw;

    operator u8() { return raw; }

    Mapper1PrgBankRegister& operator=(u8 value) { raw = value; return *this; }
};

struct Mapper1Registers
{
    Mapper1ControlRegister control;
    u8 chrBank0;
    u8 chrBank1;
    Mapper1PrgBankRegister prgBank;
};