#include "Mapper7.hpp"

Mapper7::Mapper7(std::vector<u8> &&prgRom, std::vector<u8> &&chrRom)
    : Mapper(std::move(prgRom), std::move(chrRom), MirroringType::SingleScreenLow)
    , bankSelectRegister(0)
{
    if (this->chrRom.empty()) {
        this->chrRom.resize(0x2000);
    }
}

void Mapper7::write(u16 addr, u8 value)
{
    if (addr < 0x2000) {
        chrRom[addr] = value;
    } else if (addr >= 0x6000 && addr < 0x8000) {
        prgRam[addr - 0x6000] = value;
    } else if (addr >= 0x8000 && addr < 0xFFFF) {
        bankSelectRegister = value & 7;
        mirroringType = value & 0x10 ? MirroringType::SingleScreenHigh : MirroringType::SingleScreenLow;
    }
}

u8 Mapper7::read(u16 addr)
{
    if (addr < 0x2000) {
        return chrRom[addr];
    } else if (addr >= 0x6000 && addr < 0x8000) {
        return prgRam[addr - 0x6000];
    } else if (addr >= 0x8000 && addr < 0xFFFF) {
        const unsigned PRG_ROM_BANK_SIZE = 0x8000;
        auto prgRomAddr = (bankSelectRegister * PRG_ROM_BANK_SIZE) + (addr - 0x8000);
        return prgRom[prgRomAddr];
    }
    return 0;
}

MirroringType Mapper7::getMirroringType()
{
    return mirroringType;
}
