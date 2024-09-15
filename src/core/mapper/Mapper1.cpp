#include "Mapper1.hpp"

Mapper1::Mapper1(std::vector<u8> &&prgRom, std::vector<u8> &&chrRom, MirroringType mirroringType)
    : Mapper(std::move(prgRom), std::move(chrRom), mirroringType)
    , shiftRegister(0x10)
    , registers()
    , writableChr(false)
{
    if (this->chrRom.empty()) {
        this->chrRom.resize(0x2000);
        writableChr = true;
    }
    registers.control.prgRomBankMode = 3;
}

void Mapper1::write(u16 addr, u8 value)
{
    if (writableChr && addr < 0x2000) {
        auto address = absoluteChrAddress(addr);
        chrRom[address] = value;
    } else if (addr >= 0x6000 && addr < 0x8000) {
        prgRam[addr - 0x6000] = value;
    } else if (addr >= 0x8000 && addr <= 0xFFFF) {
        // MMC1 Serial port
        writeToLoadRegister(addr, value);
    }
}

u8 Mapper1::read(u16 addr)
{
    if (addr < 0x2000) {
        auto address = absoluteChrAddress(addr);
        return chrRom[address];
    } else if (addr >= 0x6000 && addr < 0x8000) {
        return prgRam[addr - 0x6000];
    } else if (addr >= 0x8000 && addr <= 0xFFFF) {
        auto address = absolutePrgAddress(addr - 0x8000);
        return prgRom[address];
    }
    return 0;
}

MirroringType Mapper1::getMirroringType()
{
    using enum MirroringType;
    switch (registers.control.mirroring)
    {
        case 0:
            return SingleScreenLow;
        case 1:
            return SingleScreenHigh;
        case 2:
            return Vertical;
        case 3:
            return Horizontal;
    }
    return mirroringType;
}

void Mapper1::resetShiftRegister()
{
    shiftRegister = 0x10;
}

void Mapper1::writeToLoadRegister(u16 addr, u8 value)
{
    if (value & 0x80) {
        resetShiftRegister();
        registers.control.prgRomBankMode = 3;
    } else if (shiftRegister & 0x01) {
        // TODO: Ignore consecutive cycle writes to shift register
        shiftRegister >>= 1;
        shiftRegister |= (value & 1) << 4;
        if (addr < 0xA000) {
            registers.control = shiftRegister;
        } else if (addr < 0xC000) {
            registers.chrBank0 = shiftRegister;
        } else if (addr < 0xE000) {
            registers.chrBank1 = shiftRegister;
        } else {
            registers.prgBank = shiftRegister;
        }
        resetShiftRegister();
    } else {
        shiftRegister >>= 1;
        shiftRegister |= (value & 1) << 4;
    }
}

unsigned Mapper1::absoluteChrAddress(u16 addr)
{
    const auto& chrBankMode = registers.control.chrRomBankMode;
    if (chrBankMode == 0) {
        auto bank = (registers.chrBank0 >> 1);
        return bank * 0x2000 + addr;
    } else {
        if (addr < 0x1000) {
            auto bank = registers.chrBank0;
            return bank * 0x1000 + addr;
        } else {
            auto bank = registers.chrBank1;
            return bank * 0x1000 + addr - 0x1000; 
        }
    }
}

unsigned Mapper1::absolutePrgAddress(u16 addr)
{
    const auto& prgBankMode = registers.control.prgRomBankMode;
    if (prgBankMode < 2) {
        auto bank = (registers.prgBank >> 1);
        return bank * 0x8000 + addr;
    } else if (prgBankMode == 2) {
        if (addr < 0x4000) {
            return addr;
        } else {
            auto bank = registers.prgBank;
            return bank * 0x4000 + addr;
        }
    } else {
        if (addr < 0x4000) {
            auto bank = registers.prgBank;
            return bank * 0x4000 + addr;
        } else {
            return prgRom.size() - 0x8000 + addr;
        }
    }
}
