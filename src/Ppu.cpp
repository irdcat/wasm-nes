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
    offsetToggleLatch = false;
}

u8 Ppu::read(u8 index)
{
    u8 result = openBusContents;
    if(index == 2) { // 0x2002 PPUSTATUS - Ppu status register
        result = registers.ppuStatus | (openBusContents & 0x1F);
        registers.ppuStatus.inVBlank = false;
        offsetToggleLatch = false;
    } else if (index == 4) { // 0x2004 OAMDATA - OAM data port
        result = oam.raw[registers.oamAddr];
        refreshOpenBus(result);
    } else if (index == 7) { // 0x2007 PPUDATA - Ppu data register
        auto ppuData = ppuRead(registers.ppuAddr);
        if((registers.ppuAddr & 0x3F00) == 0x3F00) {
            result = (openBusContents & 0xC0) | (ppuData & 0x3F);
        }
        vramReadBuffer = ppuData;
        refreshOpenBus(result);
        registers.ppuAddr = registers.ppuAddr + (registers.ppuCtrl.vramAddressIncrement ? 32 : 1);
    }
    return result;
}

void Ppu::write(u8 index, u8 data)
{
    refreshOpenBus(data);
    if(index == 0) { // 0x2000 PPUCTRL - Ppu control register
        auto oldVBlankNmi = registers.ppuCtrl.VBlankNmi;
        registers.ppuCtrl = data;
        if(!oldVBlankNmi && registers.ppuCtrl.VBlankNmi && registers.ppuStatus.inVBlank) {
            triggerNmi();
        }
    } else if (index == 1) { // 0x2001 PPUMASK - Ppu mask register
        registers.ppuMask = data;
    } else if (index == 3) { // 0x2003 OAMADDR - OAM address port
        registers.oamAddr = data;
    } else if (index == 4) { // 0x2004 OAMDATA - OAM data port
        oam.raw[registers.oamAddr++] = data;
    } else if (index == 5) { // 0x2005 PPUSCROLL - Ppu scrolling position register (X Scroll on first write, Y Scroll on second write)
        if(offsetToggleLatch) {
            registers.ppuScroll.y = data;
        } else {
            registers.ppuScroll.x = data;
        }
        offsetToggleLatch = !offsetToggleLatch;
    } else if (index == 6) { // 0x2006 PPUADDR - Ppu address register (MSB on first write, LSB on second write)
        if(offsetToggleLatch) {
            registers.ppuAddr.low = data;
        } else {
            registers.ppuAddr.high = data;
        }
        offsetToggleLatch = !offsetToggleLatch;
    } else if (index == 7) { // 0x2007 PPUDATA - Ppu data register
        ppuWrite(registers.ppuAddr, data);
        registers.ppuAddr = registers.ppuAddr + (registers.ppuCtrl.vramAddressIncrement ? 32 : 1);
    }
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

void Ppu::refreshOpenBus(u8 value)
{
    openBusDecayTimer = 77777;
    openBusContents = value;
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
