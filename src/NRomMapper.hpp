#pragma once

#include <vector>

#include "CartridgeMapper.hpp"

class NRomMapper : public CartridgeMapper
{
    public:
        NRomMapper(unsigned ramSize, unsigned romSize, unsigned chrRomSize);

        ~NRomMapper() = default;

        void write(u16 addr, u8 value) override;

        u8 read(u16 addr) override;

    private:
        std::vector<u8> prgRom;
        std::vector<u8> chrRom;
        std::vector<u8> prgRam;

        u8& memoryRef(u16 addr);
};