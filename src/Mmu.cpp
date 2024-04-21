#include "Mmu.hpp"

Mmu::Mmu()
    : resetSignalled(false)
{
}

Mmu::Mmu(const std::shared_ptr<Ppu> &ppu, const std::shared_ptr<Cartridge> &cartridge)
    : ppu(ppu), cartridge(cartridge), internalRam(), resetSignalled(false)
{
}

u8 Mmu::readFromMemory(u16 addr)
{
    tick();
    u8 result = 0;
    if(addr < 0x2000) {
        result = internalRam[addr & 0x7FF];
    } else if(addr < 0x4000) {
        result = ppu->read(addr & 7);
    } else if(addr < 0x4018) {
        // TODO: Readable MMIO Registers
    } else {
        result = cartridge->read(addr);
    }
    return result;
}

void Mmu::writeIntoMemory(u16 addr, u8 value)
{
    if(resetSignalled) {
        readFromMemory(addr);
        return;
    }

    tick();
    if(addr < 0x2000) {
        internalRam[addr & 0x7FF] = value;
    } else if(addr < 0x4000) {
        ppu->write(addr & 7, value);
    } else if(addr < 0x4018) {
        auto mmioAddr = addr & 0x1F;
        if(mmioAddr == 0x14) {
            for(unsigned byte = 0; byte < 256; byte++) {
                writeIntoMemory(0x2004, readFromMemory((value & 7) * 0x100 + byte));
            }
        }
        // TODO: Writable MMIO Registers
    } else {
        cartridge->write(addr, value);
    }
}

void Mmu::signalReset(bool signal)
{
    resetSignalled = signal;
}

unsigned Mmu::getAndResetTickCounterValue()
{
    auto old = tickCounter;
    tickCounter = 0;
    return old;
}

void Mmu::tick()
{
    tickCounter++;
    for(auto i = 0; i < 3; i++) {
        ppu->tick();
    }
    // TODO: APU cycle triggering
}
