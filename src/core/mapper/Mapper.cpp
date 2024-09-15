#include "Mapper.hpp"

Mapper::Mapper(std::vector<u8> &&prgRom, std::vector<u8> &&chrRom, MirroringType mirroringType)
    : prgRom(prgRom)
    , chrRom(chrRom)
    , prgRam()
    , mirroringType(mirroringType)
{
}