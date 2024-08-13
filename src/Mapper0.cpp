#include "Mapper0.hpp"

Mapper0::Mapper0(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType)
    : Mapper(std::move(prgRom), std::move(chrRom), mirroringType)
    , writableChrRom(false)
{
    if(this->chrRom.empty()) {
        this->chrRom.resize(0x2000);
        this->writableChrRom = true;
    }
}

void Mapper0::write(u16 addr, u8 value)
{
    if(addr < 0x2000 && !writableChrRom) {
        return;
    }
    if(addr >= 0x8000 && addr < 0xFFFF) {
        return;
    }

    auto& mem = memoryRef(addr);
    mem = value;
}

u8 Mapper0::read(u16 addr)
{
    return memoryRef(addr);
}

MirroringType Mapper0::getMirroringType()
{
    return mirroringType;
}

u8 &Mapper0::memoryRef(u16 addr)
{
    static u8 dummyByte = 0;
    if(addr < 0x2000 && chrRom.size() != 0) {
        return chrRom[addr % chrRom.size()];
    } else if (addr >= 0x6000 && addr < 0x8000) {
        auto prgRamAddr = (addr - 0x6000) % prgRam.size();
        return prgRam[prgRamAddr];
    } else if (addr >= 0x8000 && prgRom.size() != 0) {
        auto prgRomAddr = (addr - 0x8000) % prgRom.size();
        return prgRom[prgRomAddr];
    }

    return dummyByte;
}
