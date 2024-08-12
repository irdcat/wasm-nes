#pragma once

#include "Mapper.hpp"

/**
 * UxROM
 */
class Mapper2 : public Mapper
{
    public:
        Mapper2(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType);

        ~Mapper2() = default;

        void write(u16 addr, u8 value) override;

        u8 read(u16 addr) override;

        MirroringType getMirroringType() override;

    private:
        u8& memoryRef(u16 addr);
        u8 bankSelectRegister;
};