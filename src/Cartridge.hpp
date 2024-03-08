#pragma once

#include <fstream>
#include <vector>
#include <array>

#include "Types.hpp"

class Cartridge
{
    public:
        Cartridge() = default;

        bool loadFromFile(std::ifstream file);

        void write(u16 addr, u8 value);

        u8 read(u16 addr);

        const std::vector<u8>& getChrRom();

    private:
        std::vector<u8> prgRom;
        std::vector<u8> chrRom;
        std::array<u8, 0x2000> prgRam;
        std::array<u8, 0x1000> chrRam;

        std::array<u16, 4> nta;
        bool usesChrRamInsteadOfChrRom;

        u8& memoryRef(u16 addr);
};