#include "Cartridge.hpp"

bool Cartridge::loadFromFile(std::ifstream file)
{
    if(!file.is_open()) {
        return false;
    }
    
    std::array<u8, 16> headerData;
    file.read(reinterpret_cast<char*>(&headerData[0]), 16);

    auto nesHeaderData = parseNesHeader(headerData);

    prgRom.resize(nesHeaderData->prgRomBanks * 0x4000);
    if(nesHeaderData->chrRomBanks > 0) {
        chrRom.resize(nesHeaderData->chrRomBanks * 0x2000);
        usesChrRamInsteadOfChrRom = false;
    } else {
        chrRom.resize(0x2000);
        usesChrRamInsteadOfChrRom = true;
    }

    using enum MirroringType;
    switch (nesHeaderData->mirroring)
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

    if(nesHeaderData->trainer) {
        file.ignore(512);
    }

    file.read(reinterpret_cast<char*>(prgRom.data()), prgRom.size());
    file.read(reinterpret_cast<char*>(chrRom.data()), chrRom.size());

    return true;
}

void Cartridge::write(u16 addr, u8 value)
{
    if(addr < 0x2000 && !usesChrRamInsteadOfChrRom) {
        return;
    }
    if (addr >= 0x8000 && addr < 0xFFFF) {
        return;
    }

    auto& ref = memoryRef(addr);
    ref = value;
}

u8 Cartridge::read(u16 addr)
{
    return memoryRef(addr);
}

const std::vector<u8>& Cartridge::getChrRom()
{
    return chrRom;
}

u8 &Cartridge::memoryRef(u16 addr)
{
    static u8 dummyByte = 0;
    if (addr < 0x2000 && chrRom.size() != 0) {
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
        auto prgRamAddr = (addr - 0x6000) % prgRam.size();
        return prgRam[prgRamAddr];
    } else if (addr >= 0x8000 && prgRom.size() != 0) {
        auto prgRomAddr = (addr - 0x8000) % prgRom.size();
        return prgRom[prgRomAddr]; 
    }

    return dummyByte;
}

std::unique_ptr<Cartridge::NesHeaderData> Cartridge::parseNesHeader(const NesHeader &nesHeader)
{
    using enum MirroringType;
    if(!isValidNesHeader(nesHeader)) {
        return std::unique_ptr<NesHeaderData>(nullptr);
    }
    auto flags6 = nesHeader[6];
    auto flags7 = nesHeader[7];

    NesHeaderData headerData = {
        .prgRomBanks = nesHeader[4],
        .chrRomBanks = nesHeader[5],
        .mapperNo = static_cast<unsigned>((flags6 >> 4) | (flags7 & 0xF0)),
        .persistentMemory = static_cast<bool>(((flags6 & 0x2) >> 1)),
        .trainer = static_cast<bool>(((flags6 & 0x4) >> 2)),
        .mirroring = ((flags6 & 0x8) >> 3) ? FourScreen : ((flags6 & 0x1) ? Horizontal : Vertical)
    };

    return std::make_unique<NesHeaderData>(headerData);
}

bool Cartridge::isValidNesHeader(const NesHeader &nesHeader)
{
    // So far only iNES 1.0 header is supported
    return std::string(&nesHeader[0], &nesHeader[4]) == "NES\x1A";
}
