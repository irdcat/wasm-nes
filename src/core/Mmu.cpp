#include "Mmu.hpp"

Mmu::Mmu()
    : resetSignalled(false)
{
}

Mmu::Mmu(const std::shared_ptr<Ppu> &ppu,
         const std::shared_ptr<Apu> &apu,
         const std::shared_ptr<Cartridge> &cartridge, 
         const std::shared_ptr<Controllers> &controllers)
    : ppu(ppu)
    , apu(apu)
    , cartridge(cartridge)
    , controllers(controllers)
    , internalRam()
    , resetSignalled(false)
    , tickCounter(0)
{
}

/**
 * Reads the from memory as it is mapped for CPU 
 * and returns the byte that is lying beneath the given address.
 */
u8 Mmu::readFromMemory(u16 addr)
{
    // Every read from memory triggers a tick of the other peripherials
    // Benefit of that approach is that ticks can be precisely triggered
    // in the middle of instruction execution.
    tick();
    
    // Every read from unmapped memory location defaults to 0
    // It is arbitrary value. On a real hardware it might be garbage
    // and games should not rely on that as it would be serious design flaw.
    u8 result = 0;

    if(addr < 0x2000) {
        // NES RAM is only 2KB big but spanned over the 8KB address space.
        // Some of the bits of the address are unused and it can be easily implemented 
        // by wrapping the address over 2KB.
        // RAM is "Mirrored". 
        result = internalRam[addr & 0x7FF];
    } else if(addr < 0x4000) {
        // Similarly as NES RAM, the 8 PPU MMIO (Memory Mapped IO) registers are mirrored
        // inside 8KB address space.
        result = ppu->read(addr & 7);
    } else if(addr < 0x4018) {
        auto mmioAddr = addr & 0x1F;
        switch(mmioAddr) {
            case 0x15:
                result = apu->read();
            case 0x16:
            case 0x17:
                result = controllers->read(mmioAddr & 0x1);
            default:
                break;
        }
    } else {
        // The rest of the address space belongs to the cartridge, but some of it is unused
        result = cartridge->read(addr);
    }
    return result;
}

/**
 * Writes byte into memory as it is mapped for CPU
 * into the location specified by given address.
 */
void Mmu::writeIntoMemory(u16 addr, u8 value)
{
    // All writes are turned into reads during reset
    if(resetSignalled) {
        readFromMemory(addr);
        return;
    }

    // Every write to memory triggers a tick of the other peripherials
    // Benefit of that approach is that ticks can be precisely triggered
    // in the middle of instruction execution.
    tick();
    if(addr < 0x2000) {
        // NES RAM is only 2KB big but spanned over the 8KB address space.
        // Some of the bits of the address are unused and it can be easily implemented 
        // by wrapping the address over 2KB.
        // RAM is "Mirrored".
        internalRam[addr & 0x7FF] = value;
    } else if(addr < 0x4000) {
        // Similarly as NES RAM, the 8 PPU MMIO (Memory Mapped IO) registers are mirrored
        // inside 8KB address space.
        ppu->write(addr & 7, value);
    } else if(addr < 0x4018) {
        auto mmioAddr = addr & 0x1F;
        const u16 OAMDATA_ADDRESS = 0x2004;
        const u16 pageStart = (value & 7) * 0x100;
        const u16 pageEnd = pageStart + 0xFF;
        switch(mmioAddr) {
            // Register under the address 0x4014 is a write-only OAM DMA register
            // It doesn't hold any value and it's purpose is to trigger process called OAM DMA
            // What it does is it tells the system to copy whole page of RAM memory (256 bytes)
            // into PPU's OAM (Object Attribute Memory) 
            // which holds informations about sprites to be rendered.
            // Page is specified by value that is written to the OAM DMA address.
            case 0x14:
                // Due to the behaviour of DMA (Direct Memory Access) unit, 
                // before proceeding with copying DMA attempts to halt the CPU which causes a delay of up to 3 CPU cycles.
                // Amount of cycles it takes to halt the CPU depends on instruction which triggers DMA.
                // This behaviour is not implemented here and whole process is kept to take constant 512 cycles in total.
                for(u16 dmaAddress = pageStart; dmaAddress <= pageEnd; dmaAddress++) {
                    writeIntoMemory(OAMDATA_ADDRESS, readFromMemory(dmaAddress));
                }
                break;

            case 0x16:
                controllers->strobe(value);
                break;
            
            default:
                apu->write(mmioAddr, value);
                break;
        }
    } else {
        // The rest of the address space belongs to the cartridge, but some of it is unused
        cartridge->write(addr, value);
    }
}

/**
 * Sets the flag telling the MMU whether system is resetting.
 * Purpose of this is to turn writes into reads during reset. 
 */
void Mmu::signalReset(bool signal)
{
    resetSignalled = signal;
}

/**
 * Gets and resets tick counter value 
 * that is used to keep the execution speed of the system at desired rate. 
 */
unsigned Mmu::getAndResetTickCounterValue()
{
    auto old = tickCounter;
    tickCounter = 0;
    return old;
}

/**
 * Triggers a tick of the CPU peripherials.
 * This is a cheap way of synchronizing things. 
 * In current setup CPU controls rate of peripherials execution via this method. 
 * On a real hardware peripherials are independently connected to a external clock,
 * however it is known that PPU is running at speed 3 times faster than CPU and APU.
 * APU has the same speed as the CPU, because it is embedded into CPU chip and shares the same clock signal. 
 */
void Mmu::tick()
{
    tickCounter++;
    for(auto i = 0; i < 3; i++) {
        ppu->tick();
    }
    apu->tick();
}
