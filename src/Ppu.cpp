#include "Ppu.hpp"
#include "Cpu.hpp"

Ppu::Ppu(const std::shared_ptr<Cpu>& cpu, const std::shared_ptr<Cartridge>& cartridge)
    : cpuWeak(cpu)
    , cartridgeWeak(cartridge)
{
    openBusDecayTimer = 0;
    openBusContents = 0;
    vramReadBuffer = 0;
    scanline = -1;
    renderingPositionX = 0;
}

u8 Ppu::read(u8 index)
{
    return access<0>(index);
}

void Ppu::write(u8 index, u8 data)
{
    access<1>(index, data);
}

void Ppu::tick()
{
    if(openBusDecayTimer > 0) {
        openBusDecayTimer--;
        if(openBusDecayTimer == 0) {
            openBusContents = 0;
        }
    }

    renderingPositionX++;
    if(renderingPositionX >= 341) {
        
        renderingPositionX = 0;
        scanline++;

        if(scanline == 241) {
            registers.ppuStatus.inVBlank = 1;
            registers.ppuStatus.spriteZeroHit = 0;
            triggerNmi();
        }

        if(scanline == 262) {
            scanline = 0;
            registers.ppuStatus.spriteZeroHit = 0;
            registers.ppuStatus.inVBlank = 0;
        }
    }
}

void Ppu::triggerNmi()
{
    auto cpu = cpuWeak.lock();
    cpu->interrupt(InterruptType::NMI);
}

u8 Ppu::ppuRead(u16 addr)
{
    addr &= 0x3FFF;
    if(addr >= 0x3F00) {
        addr = addr % 4 == 0 ? addr & 0xF : addr & 0x1F;
        return palette[addr];
    }

    auto cartridge = cartridgeWeak.lock();
    return cartridge->read(addr);
}

void Ppu::ppuWrite(u16 addr, u8 value)
{
    addr &= 0x3FFF;
    if(addr >= 0x3F00) {
        addr = addr % 4 == 0 ? addr & 0xF : addr & 0x1F;
        palette[addr] = value;
        return;
    }

    auto cartridge = cartridgeWeak.lock();
    cartridge->write(addr, value);
}
