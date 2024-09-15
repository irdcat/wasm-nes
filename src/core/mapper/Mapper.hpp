#pragma once

#include <array>
#include <vector>

#include "../Types.hpp"
#include "../MirroringType.hpp"

class Mapper
{
    public:
        Mapper(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType);

        virtual ~Mapper() = default;

        virtual void write(u16 addr, u8 value) = 0;

        virtual u8 read(u16 addr) = 0;

        virtual MirroringType getMirroringType() = 0;

    protected:
        std::vector<u8> prgRom;
        std::vector<u8> chrRom;
        std::array<u8, 0x2000> prgRam;
        MirroringType mirroringType;
};