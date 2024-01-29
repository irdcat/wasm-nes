#include "Cartridge.hpp"

bool Cartridge::loadFromFile(std::string filename)
{
    // TODO: Implement reading cart from file
    return false;
}

void Cartridge::write(u16 addr, u8 value)
{
    if (addr < 0x2000 || (addr >= 0x8000 && addr < 0xFFFF)) {
        return;
    }

    auto& ref = memoryRef(addr);
    ref = value;
}

u8 Cartridge::read(u16 addr)
{
    return memoryRef(addr);
}

u8 &Cartridge::memoryRef(u16 addr)
{
    static u8 dummyByte = 0;
    if (addr < 0x2000) {
        return chrRom[addr % chrRom.size()];
    } else if (addr < 0x3F00) {
        if(addr >= 0x3000) {
            addr -= 0x1000;
        }
        auto nametableIndex = (addr >> 10) & 0x3;
        auto nametableAddress = nta[nametableIndex];
        auto effectiveAddress = nametableAddress + (addr & 0x3FF);
        return chrRam[effectiveAddress];
    } else if (addr >= 0x6000 && addr < 0x8000) {
        auto prgRamAddr = addr - 0x6000;
        return prgRam[prgRamAddr];
    } else if (addr >= 0x8000 && addr < 0xFFFF) {
        auto prgRomAddr = addr - 0x8000;
        return prgRom[prgRomAddr]; 
    }

    return dummyByte;
}
