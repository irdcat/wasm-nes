template <bool IsWrite>
inline u8 Mmu::memoryAccess(u16 addr, u8 value)
{
    if(IsWrite && resetSignalled) {
        return memoryAccess<0>(addr, value);
    }

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
        auto mmioAddr = addr & 0x1F;
        switch (mmioAddr)
        {
            case 0x14:
                if(IsWrite) {
                    for(unsigned byte = 0; byte < 256; byte++) {
                        memoryAccess<1>(0x2004, memoryAccess<0>((value & 7) * 0x100 + byte));
                    }
                }
                break;
            // TODO: Rest of the MMIO registers
            default:
                break;
        }
    } else {
        if constexpr (!IsWrite) {
            return cartridge->read(addr);
        }
        cartridge->write(addr, value);
    }
    return 0;
}