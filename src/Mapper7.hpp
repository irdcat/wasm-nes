#pragma once

#include "Mapper.hpp"

class Mapper7 : public Mapper
{
    public:
        Mapper7(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom);

        ~Mapper7() = default;

        void write(u16 addr, u8 value) override;

        u8 read(u16 addr) override;

        MirroringType getMirroringType() override;

    private:
        u8 bankSelectRegister;
};