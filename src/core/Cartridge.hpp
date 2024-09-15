#pragma once

#include <fstream>
#include <vector>
#include <memory>
#include <array>

#include "Types.hpp"
#include "MirroringType.hpp"
#include "mapper/Mapper.hpp"

class Cartridge
{
    public:
        Cartridge() = default;

        bool loadFromFile(std::ifstream file);

        void write(u16 addr, u8 value);

        u8 read(u16 addr);

        MirroringType getMirroringType() const;

    private:
        std::unique_ptr<Mapper> mapper;

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

        bool assignMapper(unsigned mapperNo, std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType);
};