#pragma once

#include <functional>

#include "Mapper.hpp"
#include "Mapper4Registers.hpp"

class Mapper4 : public Mapper
{
    public:
        Mapper4(std::vector<u8>&& prgRom, std::vector<u8>&& chrRom, MirroringType mirroringType, const std::function<void()>& irqCallback);

        ~Mapper4() = default;

        void write(u16 addr, u8 value) override;

        u8 read(u16 addr) override;

        MirroringType getMirroringType() override;

        void tickScanlineIrq() override;
    
    private:
        std::function<void()> irqCallback;
        Mapper4Registers registers;
        std::array<u32, 4> basePrgBankAddresses;
        std::array<u32, 8> baseChrBankAddresses;
        std::array<u8, 0x1000> chrRam;
        bool irqReloadPending;

        void updateBankMapping();
};