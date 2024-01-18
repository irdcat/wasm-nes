#include "Mmu.hpp"

u8 Mmu::readFromMemory(u16 addr)
{
    if(addr < 0x2000)
    {
        // Internal RAM mirrored up to address 1FFF every 2KB.
        return internalRam[addr % 0x800];
    }
    return u8();
}

void Mmu::writeIntoMemory(u16 addr, u8 value)
{
    if(addr < 0x2000)
    {
        // Internal RAM mirrored up to address 1FFF every 2KB.
        internalRam[addr % 0x800] = value;
    }
}
