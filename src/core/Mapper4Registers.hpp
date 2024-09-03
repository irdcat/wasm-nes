#pragma once

#include <array>

#include "RegisterBit.hpp"
#include "MirroringType.hpp"

union Mapper4BankSelectRegister
{
    RegisterBit<0, 3> bank;
    RegisterBit<5> prgRamEnable;
    RegisterBit<6> prgRomBankMode;
    RegisterBit<7> chrA12Inversion;
    u8 raw;

    operator u8() { return raw; }

    Mapper4BankSelectRegister& operator=(u8 value) { raw = value; return *this; }
};

struct Mapper4Registers
{
    Mapper4BankSelectRegister bankSelect;
    std::array<u8, 8> bankRegister;
    MirroringType mirroring;
    u8 irqCounter;
    u8 irqReload;
    bool irqEnable;
};