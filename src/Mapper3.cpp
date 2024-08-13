#include "Mapper3.hpp"

Mapper3::Mapper3(std::vector<u8> &&prgRom, std::vector<u8> &&chrRom, MirroringType mirroringType)
    : Mapper(std::move(prgRom), std::move(chrRom), mirroringType)
    , bankSelectRegister(0)
{
}

void Mapper3::write(u16 addr, u8 value)
{
    if(addr >= 0x8000 && addr <= 0xFFFF) {
        bankSelectRegister = value & 0x3;
    } else if(addr >= 0x6000 && addr < 0x8000) {
        prgRam[addr - 0x6000] = value;
    }
}

u8 Mapper3::read(u16 addr)
{
    const unsigned CHR_ROM_BANK_SIZE = 0x2000;
    if (addr < 0x2000) {
        auto chrRomAddr = CHR_ROM_BANK_SIZE * bankSelectRegister + addr;
        return chrRom[chrRomAddr];
    } else if (addr >= 0x6000 && addr < 0x8000) {
        auto prgRamAddr = addr - 0x6000;
        return prgRam[prgRamAddr];
    } else if (addr >= 0x8000 && addr < 0xFFFF) {
        auto prgRomAddr = addr - 0x8000;
        return prgRom[prgRomAddr];
    }
    return 0;
}

MirroringType Mapper3::getMirroringType()
{
    return mirroringType;
}
