#include "Mapper4.hpp"

Mapper4::Mapper4(std::vector<u8> &&prgRom, std::vector<u8> &&chrRom, MirroringType mirroringType, const std::function<void()>& irqCallback)
    : Mapper(std::move(prgRom), std::move(chrRom), mirroringType)
    , irqCallback(irqCallback)
    , registers()
    , chrRam()
    , irqReloadPending(false)
{
    basePrgBankAddresses[0] = prgRom.size() - 0x4000;
    basePrgBankAddresses[1] = prgRom.size() - 0x2000;
    basePrgBankAddresses[2] = prgRom.size() - 0x4000;
    basePrgBankAddresses[3] = prgRom.size() - 0x2000;

    for(auto& chrBankAddress : baseChrBankAddresses) {
        chrBankAddress = chrRom.size() - 0x400;
    }
    baseChrBankAddresses[0] = chrRom.size() - 0x800;
    baseChrBankAddresses[3] = chrRom.size() - 0x800;
}

void Mapper4::write(u16 addr, u8 value)
{
    bool oddAddress = addr & 0x1;
    if (addr >= 0x2000 && addr < 0x3000) {
        chrRam[addr - 0x2000] = value;
    } else if (addr >= 0x6000 && addr < 0x8000) {
        prgRam[addr - 0x6000] = value;
    } else if (addr >= 0x8000 && addr < 0xA000) {
        if(oddAddress) {
            auto bank = registers.bankSelect;
            registers.bankRegister[bank] = value;
            updateBankMapping();
        } else {
            registers.bankSelect = value;
        }
    } else if (addr >= 0xA000 && addr < 0xC000) {
        if (!oddAddress) {
            auto mirroring = (value & 0x1) ? MirroringType::Horizontal : MirroringType::Vertical;
            registers.mirroring = mirroring;
        }
        // Odd address points to PRG RAM protect register
        // In practive this is not needed in this emulator
    } else if (addr >= 0xC000 && addr < 0xE000) {
        if (oddAddress) {
            registers.irqCounter = registers.irqReload;
        } else {
            registers.irqCounter = 0;
            irqReloadPending = true;
        }
    } else if (addr >= 0xE000) {
        registers.irqEnable = oddAddress;
    }
}

u8 Mapper4::read(u16 addr)
{
    if (addr < 0x2000) {
        const auto chrBank = (addr >> 10) & 0x7;
        const auto relativeAddress = addr & 0x3FF;
        const auto absoluteAddress = baseChrBankAddresses[chrBank] + relativeAddress;
        return chrRom[absoluteAddress];
    } else if(addr < 0x3000) {
        return chrRam[addr - 0x2000];
    } else if (addr >= 0x6000 && addr < 0x8000) {
        return prgRam[addr - 0x6000];
    } else if (addr >= 0x8000) {
        const auto prgBank = (addr >> 13) & 3;
        const auto relativeAddress = addr & 0x1FFF;
        const auto absoluteAddress = basePrgBankAddresses[prgBank] + relativeAddress;
        return prgRom[absoluteAddress];
    }
    return 0;
}

MirroringType Mapper4::getMirroringType()
{
    if (mirroringType == MirroringType::FourScreen) {
        return mirroringType;
    }

    return registers.mirroring;
}

void Mapper4::tickScanlineIrq()
{
    bool zeroTransition = false;
    if (registers.irqCounter == 0 || irqReloadPending) {
        registers.irqCounter = registers.irqReload;
        irqReloadPending = false;
    } else {
        registers.irqCounter--;
        zeroTransition = registers.irqCounter == 0;
    }

    if (zeroTransition && registers.irqEnable) {
        irqCallback();
    }
}

void Mapper4::updateBankMapping()
{
    if (registers.bankSelect.chrA12Inversion) {
        baseChrBankAddresses[0] = (registers.bankRegister[0] & 0xFE) * 0x400;
        baseChrBankAddresses[1] = (registers.bankRegister[0] & 0xFE) * 0x400 + 0x400;
        baseChrBankAddresses[2] = (registers.bankRegister[1] & 0xFE) * 0x400;
        baseChrBankAddresses[3] = (registers.bankRegister[1] & 0xFE) * 0x400 + 0x400;
        baseChrBankAddresses[4] = registers.bankRegister[2] * 0x400;
        baseChrBankAddresses[5] = registers.bankRegister[3] * 0x400;
        baseChrBankAddresses[6] = registers.bankRegister[4] * 0x400;
        baseChrBankAddresses[7] = registers.bankRegister[5] * 0x400;
    } else {
        baseChrBankAddresses[0] = registers.bankRegister[2] * 0x400;
        baseChrBankAddresses[1] = registers.bankRegister[3] * 0x400;
        baseChrBankAddresses[2] = registers.bankRegister[4] * 0x400;
        baseChrBankAddresses[3] = registers.bankRegister[5] * 0x400;
        baseChrBankAddresses[4] = (registers.bankRegister[0] & 0xFE) * 0x400;
        baseChrBankAddresses[5] = (registers.bankRegister[0] & 0xFE) * 0x400 + 0x400;
        baseChrBankAddresses[6] = (registers.bankRegister[1] & 0xFE) * 0x400;
        baseChrBankAddresses[7] = (registers.bankRegister[1] & 0xFE) * 0x400 + 0x400;
    }

    if (registers.bankSelect.prgRomBankMode) {
        basePrgBankAddresses[0] = prgRom.size() - 0x4000;
        basePrgBankAddresses[1] = registers.bankRegister[7] & 0x3F * 0x2000;
        basePrgBankAddresses[2] = registers.bankRegister[6] & 0x3F * 0x2000;
        basePrgBankAddresses[3] = prgRom.size() - 0x2000;
    } else {
        basePrgBankAddresses[0] = registers.bankRegister[6] & 0x3F * 0x2000;
        basePrgBankAddresses[1] = registers.bankRegister[7] & 0x3F * 0x2000;
        basePrgBankAddresses[2] = prgRom.size() - 0x4000;
        basePrgBankAddresses[3] = prgRom.size() - 0x2000;
    }
}
