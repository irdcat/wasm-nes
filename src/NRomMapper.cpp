#include "NRomMapper.hpp"

NRomMapper::NRomMapper(unsigned ramSize, unsigned prgRomSize, unsigned chrRomSize)
{
    prgRam.resize(ramSize);
    prgRom.resize(prgRomSize);
    chrRom.resize(chrRomSize);
}

void NRomMapper::write(u16 addr, u8 value)
{
    auto& ref = memoryRef(addr);
    ref = value;
}

u8 NRomMapper::read(u16 addr)
{
    return memoryRef(addr);
}

u8 &NRomMapper::memoryRef(u16 addr)
{
    u16 relativeAddress = addr - 0x6000 < 0 ? u16(0) : addr - 0x6000;
    if(addr < 0x2000) {
        return prgRam[addr % prgRam.size()];
    }
    return prgRom[(addr - 0x2000) % prgRom.size()];
}
