template <bool IsWrite>
inline u8 Mmu::memoryAccess(u16 addr, u8 value)
{
    // TODO: Turn writes into reads during reset
    tick();

    if(addr < 0x2000) {
        u8& ramByte = internalRam[addr & 0xFFF];
        if constexpr(!IsWrite) {
            return ramByte;
        }
        ramByte = value;
    } else if(addr < 0x4000) {
        auto ppuMmioAddr = addr & 7;
        if constexpr (!IsWrite) {
            return ppu->read(ppuMmioAddr);
        }
        ppu->write(ppuMmioAddr, value);
    } else if(addr < 0x4018) {
        // TODO: APU, Joypads, OAM DMA
    } else {
        // TODO: Cartridge space
    }
    return 0;
}