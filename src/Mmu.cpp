#include "Mmu.hpp"

Mmu::Mmu(const std::shared_ptr<Ppu> &ppu)
    : ppu(ppu)
{
}

u8 Mmu::readFromMemory(u16 addr)
{
    tick();
    if(addr < 0x2000) {
        // Internal RAM mirrored up to address 1FFF every 2KB.
        return internalRam[addr & 0xFFF];
    }
    if(addr < 0x4000) {
        // PPU Registers mirrored up to address 0x3FFF every 8B
        return ppu->read((addr - 0x2000) & 0x7);
    }
    return u8();
}

void Mmu::writeIntoMemory(u16 addr, u8 value)
{
    tick();
    if(addr < 0x2000) {
        // Internal RAM mirrored up to address 1FFF every 2KB.
        internalRam[addr % 0x800] = value;
    } else if(addr < 0x4000) {
        // PPU Registers mirrored up to address 0x3FFF every 8B
        ppu->write((addr - 0x2000) & 0x7, value);
    }
}

void Mmu::tick()
{
    for(auto i = 0; i < 3; i++) {
        ppu->tick();
    }
    // TODO: APU cycle triggering
}
