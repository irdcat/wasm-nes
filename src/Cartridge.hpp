#pragma once

#include <fstream>
#include <vector>
#include <memory>
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

        enum class MirroringType
        {
            Horizontal,
            Vertical,
            FourScreen
        };

        struct NesHeaderData
        {
            unsigned prgRomBanks;
            unsigned chrRomBanks;
            unsigned mapperNo;
            bool persistentMemory;
            bool trainer;
            MirroringType mirroring;
        };

        using NesHeader = std::array<u8, 16>;

        std::unique_ptr<NesHeaderData> parseNesHeader(const NesHeader& nesHeader);
        bool isValidNesHeader(const NesHeader& nesHeader);
};