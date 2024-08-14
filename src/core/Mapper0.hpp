#pragma once

#include "Mapper.hpp"

/**
 * NROM
 */
class Mapper0 : public Mapper
{
    public:
        Mapper0(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType);

        ~Mapper0() = default;

        void write(u16 addr, u8 value) override;

        u8 read(u16 addr) override;

        MirroringType getMirroringType() override;

    private:
        bool writableChrRom;
        u8& memoryRef(u16 addr);
};