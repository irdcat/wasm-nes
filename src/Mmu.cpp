#include "Mmu.hpp"

Mmu::Mmu(const std::shared_ptr<Ppu> &ppu)
    : ppu(ppu)
{
}

u8 Mmu::readFromMemory(u16 addr)
{
    return memoryAccess<0>(addr);
}

void Mmu::writeIntoMemory(u16 addr, u8 value)
{
    memoryAccess<1>(addr, value);
}

void Mmu::tick()
{
    for(auto i = 0; i < 3; i++) {
        ppu->tick();
    }
    // TODO: APU cycle triggering
}
