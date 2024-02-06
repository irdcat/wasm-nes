#include "Ppu.hpp"
#include "Cpu.hpp"

Ppu::Ppu(const std::shared_ptr<Cpu>& cpu, const std::shared_ptr<Cartridge>& cartridge)
    : cpuWeak(cpu)
    , cartridgeWeak(cartridge)
{
    openBusDecayTimer = 0;
    openBusContents = 0;
    vramReadBuffer = 0;
    scanline = 261;
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

    if(scanline < 240 || scanline == 261)
    {
        if(registers.ppuMask.showBgSp) {
            renderingTick();
        }
    }

    renderingPositionX++;
    if(renderingPositionX >= 341) {
        
        renderingPositionX = 0;
        scanline++;

        if(scanline == 241 && renderingPositionX == 1) {
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

void Ppu::renderingTick()
{
    const auto& x = renderingPositionX;
    auto shouldDecodeTile = (x >= 0 && x <= 255) || (x >= 320 && x <= 335);

    auto baseNtAddr = 0x2000 + registers.ppuCtrl.baseNametableAddress * 0x400;
    auto baseBgAddr = 0x1000 * registers.ppuCtrl.backgroundPatternTableAddress;

    auto interleave = [](auto lsb, auto msb) {
        auto pattern = lsb | (msb << 8);
        pattern = (pattern & 0xF00F) | ((pattern & 0x0F00) >> 4) | ((pattern & 0x00F0) << 4);
        pattern = (pattern & 0xC3C3) | ((pattern & 0x3030) >> 2) | ((pattern & 0x0C0C) << 2);
        pattern = (pattern & 0x9999) | ((pattern & 0x4444) >> 1) | ((pattern & 0x2222) << 1);
        return pattern;
    };

    switch (x%8)
    {
        case 0: // Point to attribute table
            attributeTableAddress = baseNtAddr + 0x3C0;
            break;

        case 1: // Nametable access
            patternTableAddress = baseBgAddr + 16 * ppuRead(attributeTableAddress);
            if(shouldDecodeTile) {
                bgShiftPattern = (bgShiftPattern >> 16) | (tilePattern << 16);
                bgShiftAttributes = (bgShiftAttributes >> 16) | ((tileAttributes * 0x5555) << 16);
            }
            break;

        case 2: // Point to nametable or attribute table
            if(shouldDecodeTile) {
                attributeTableAddress = baseNtAddr + 0x3C0;
            } else {
                attributeTableAddress = 0x2000 + (registers.ppuAddr & 0xFFF);
            }
            break;

        case 3: // Attribute table access
            if(shouldDecodeTile) {
                tileAttributes = ppuRead(attributeTableAddress) & 3;
            }
            break;

        case 5: // Background LSB
            tilePattern = ppuRead(patternTableAddress);
            break;

        case 7: // Background MSB
            tilePattern = interleave(tilePattern, (ppuRead(patternTableAddress | 8) << 8));
            break;

        default:
            break;
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
