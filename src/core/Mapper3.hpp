#pragma once

#include "Mapper.hpp"

class Mapper3 : public Mapper
{
    public:
        Mapper3(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType);

        ~Mapper3() = default;

        void write(u16 addr, u8 value) override;

        u8 read(u16 addr) override;

        MirroringType getMirroringType() override;

    private:
        u8 bankSelectRegister;
};