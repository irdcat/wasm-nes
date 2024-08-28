#pragma once

#include "Mapper.hpp"
#include "Mapper1Registers.hpp"

/**
 * MMC1
 */
class Mapper1 : public Mapper
{
    public:
        Mapper1(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType);

        ~Mapper1() = default;

        void write(u16 addr, u8 value) override;

        u8 read(u16 addr) override;

        MirroringType getMirroringType() override;

    private:
        u8 shiftRegister;
        bool writableChr;
        Mapper1Registers registers;

        void resetShiftRegister();
        void writeToLoadRegister(u16 addr, u8 value);

        unsigned absoluteChrAddress(u16 addr);
        unsigned absolutePrgAddress(u16 addr);
};