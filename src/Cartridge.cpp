#include "Cartridge.hpp"
#include "NesHeaderData.hpp"

bool Cartridge::loadFromFile(std::ifstream file)
{
    if(!file.is_open()) {
        return false;
    }
    
    std::array<u8, 16> headerData;
    file.read(reinterpret_cast<char*>(&headerData[0]), 16);

    if(headerData[0] != 0x4E || headerData[1] != 0x45 || headerData[2] != 0x53 || headerData[3] != 0x1A) {
        return false;
    }

    auto flags6 = headerData[6];
    auto flags7 = headerData[7];
    
    using enum MirroringType;

    NesHeaderData nesHeaderData = {
        .prgRomBanks = headerData[4],
        .chrRomBanks = headerData[5],
        .mapperNo = (flags6 >> 4) | (flags7 & 0xF0),
        .persistentMemory = ((flags6 & 0x2) >> 1),
        .trainer = ((flags6 & 0x4) >> 2),
        .mirroring = ((flags6 & 0x8) >> 3) ? FourScreen : ((flags6 & 0x1) ? Horizontal : Vertical)
    };

    prgRom.resize(nesHeaderData.prgRomBanks * 0x4000);
    chrRom.resize(nesHeaderData.chrRomBanks * 0x2000);
    prgRam.resize(0x2000);
    chrRam.resize(0x1000);

    switch (nesHeaderData.mirroring)
    {
        case Horizontal:
            nta[0] = nta[1] = 0;
            nta[2] = nta[3] = 0x400;
            break;
        case Vertical:
            nta[0] = nta[2] = 0;
            nta[1] = nta[3] = 0x400;
            break;
        default:
            // Handle FourScreen mirroring case
            break;
    }

    if(nesHeaderData.trainer) {
        file.ignore(512);
    }

    file.read(reinterpret_cast<char*>(&prgRom[0]), prgRom.size());
    file.read(reinterpret_cast<char*>(&chrRom[0]), chrRom.size());

    return true;
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
