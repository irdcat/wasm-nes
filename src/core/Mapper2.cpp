#include "Mapper2.hpp"

Mapper2::Mapper2(std::vector<u8> &&prgRom, std::vector<u8> &&chrRom, MirroringType mirroringType)
    : Mapper(std::move(prgRom), std::move(chrRom), mirroringType)
    , bankSelectRegister(0)
{
    if(this->chrRom.empty()) {
        this->chrRom.resize(0x2000);
    }
}

void Mapper2::write(u16 addr, u8 value)
{
    if (addr >= 0x8000 && addr < 0xFFFF) {
        bankSelectRegister = value & 0x7;
        return;
    }

    auto& mem = memoryRef(addr);
    mem = value;
}

u8 Mapper2::read(u16 addr)
{
    return memoryRef(addr);
}

MirroringType Mapper2::getMirroringType()
{
    return mirroringType;
}

u8 &Mapper2::memoryRef(u16 addr)
{
    static const unsigned PRG_ROM_BANK_SIZE = 0x4000;
    static u8 dummyByte = 0;
    if (addr < 0x2000 && chrRom.size() != 0) {
        return chrRom[addr];
    } else if (addr >= 0x6000 && addr < 0x8000) {
        auto prgRamAddr = addr - 0x6000;
        return prgRam[addr % prgRam.size()];
    } else if (addr >= 0x8000 && addr < 0xC000) {
        // Switchable PRG ROM bank
        auto prgRomAddr = ((addr - 0x8000) & 0x3FFF) | bankSelectRegister << 14;
        return prgRom[prgRomAddr];
    } else if (addr >= 0xC000 && addr < 0xFFFF) {
        // Fixed last PRG ROM bank
        const unsigned LAST_PAGE_START = prgRom.size() - PRG_ROM_BANK_SIZE;
        auto prgRomAddr = LAST_PAGE_START + (addr & 0x3FFF);
        return prgRom[prgRomAddr];
    }
    return dummyByte;
}
