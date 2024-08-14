#include "Cartridge.hpp"
#include "Mapper0.hpp"
#include "Mapper2.hpp"
#include "Mapper3.hpp"
#include "Mapper7.hpp"

bool Cartridge::loadFromFile(std::ifstream file)
{
    if(!file.is_open()) {
        return false;
    }
    
    std::array<u8, 16> headerData;
    file.read(reinterpret_cast<char*>(&headerData[0]), 16);

    auto nesHeaderData = parseNesHeader(headerData);
    if(!nesHeaderData) {
        return false;
    }

    std::vector<u8> prgRom;
    std::vector<u8> chrRom;

    prgRom.resize(nesHeaderData->prgRomBanks * 0x4000);
    chrRom.resize(nesHeaderData->chrRomBanks * 0x2000);

    if(nesHeaderData->trainer) {
        file.ignore(512);
    }

    file.read(reinterpret_cast<char*>(prgRom.data()), prgRom.size());
    file.read(reinterpret_cast<char*>(chrRom.data()), chrRom.size());

    return assignMapper(nesHeaderData->mapperNo, std::move(prgRom), std::move(chrRom), nesHeaderData->mirroring);
}

void Cartridge::write(u16 addr, u8 value)
{
    if(!mapper) {
        return;
    }
    mapper->write(addr, value);
}

u8 Cartridge::read(u16 addr)
{
    if(!mapper) {
        return 0;
    }
    return mapper->read(addr);
}

MirroringType Cartridge::getMirroringType() const
{
    return mapper->getMirroringType();
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
        .mirroring = ((flags6 & 0x8) >> 3) ? FourScreen : ((flags6 & 0x1) ? Vertical : Horizontal)
    };

    return std::make_unique<NesHeaderData>(headerData);
}

bool Cartridge::isValidNesHeader(const NesHeader &nesHeader)
{
    // So far only iNES 1.0 header is supported
    return std::string(&nesHeader[0], &nesHeader[4]) == "NES\x1A";
}

bool Cartridge::assignMapper(unsigned mapperNo, std::vector<u8> &&prgRom, std::vector<u8> &&chrRom, MirroringType mirroringType)
{
    auto result = true;
    switch(mapperNo)
    {
        case 0:
            mapper = std::make_unique<Mapper0>(std::move(prgRom), std::move(chrRom), mirroringType);
            break;

        case 2:
            mapper = std::make_unique<Mapper2>(std::move(prgRom), std::move(chrRom), mirroringType);
            break;

        case 3:
            mapper = std::make_unique<Mapper3>(std::move(prgRom), std::move(chrRom), mirroringType);
            break;

        case 7:
            mapper = std::make_unique<Mapper7>(std::move(prgRom), std::move(chrRom));
            break;

        default:
            result = false;
            break;
    }
    return result;
}
