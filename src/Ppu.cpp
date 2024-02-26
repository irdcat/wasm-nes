#include "Ppu.hpp"
#include "Cpu.hpp"

Ppu::Ppu(const std::shared_ptr<Cartridge>& cartridge, 
    const std::shared_ptr<PpuFramebuffer>& framebuffer,
    const std::function<void()>& nmiTriggerCallback,
    const std::function<void()>& vblankInterruptCallback)
    : cartridge(cartridge)
    , framebuffer(framebuffer)
    , registers()
    , openBusDecayTimer(0)
    , openBusContents(0)
    , vramReadBuffer(0)
    , scanline(261)
    , renderingPositionX(0)
    , offsetToggleLatch(false)
    , patternTableAddress(0)
    , attributeTableAddress(0)
    , tilePattern(0)
    , tileAttributes(0)
    , bgShiftPattern(0)
    , bgShiftAttributes(0)
    , oam()
    , oam2()
    , palette()
    , nmiTriggerCallback(nmiTriggerCallback)
    , vblankInterruptCallback(vblankInterruptCallback)
{
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
            nmiTriggerCallback();
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

    if(scanline < 240 || scanline == 261) {
        if(registers.ppuMask.showBgSp) {
            renderingTick();
        }
        if(scanline != 261 && renderingPositionX < 256) {
            renderPixel();
        }
    }

    if(scanline == 241 && renderingPositionX == 1) {
        registers.ppuStatus.inVBlank = 1;
        registers.ppuStatus.spriteZeroHit = 0;
        nmiTriggerCallback();
        vblankInterruptCallback();
    }

    if(scanline == 262) {
        scanline = 0;
        registers.ppuStatus.spriteZeroHit = 0;
        registers.ppuStatus.inVBlank = 0;
    }

    renderingPositionX++;
    if(renderingPositionX >= 341) {
        renderingPositionX = 0;
        scanline++;
    }
}

void Ppu::renderingTick()
{
    auto shouldDecodeTile = (renderingPositionX >= 0 && renderingPositionX <= 255) 
        || (renderingPositionX >= 320 && renderingPositionX <= 335);

    auto baseNtAddr = 0x2000 + registers.ppuCtrl.baseNametableAddress * 0x400;
    auto baseBgAddr = 0x1000 * registers.ppuCtrl.backgroundPatternTableAddress;

    auto interleave = [](auto lsb, auto msb) {
        auto pattern = lsb | (msb << 8);
        pattern = (pattern & 0xF00F) | ((pattern & 0x0F00) >> 4) | ((pattern & 0x00F0) << 4);
        pattern = (pattern & 0xC3C3) | ((pattern & 0x3030) >> 2) | ((pattern & 0x0C0C) << 2);
        pattern = (pattern & 0x9999) | ((pattern & 0x4444) >> 1) | ((pattern & 0x2222) << 1);
        return pattern;
    };

    switch (renderingPositionX % 8)
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

void Ppu::renderPixel()
{
    bool isOnEdge = renderingPositionX < 8 || renderingPositionX > 248;
    bool showBackground = registers.ppuMask.showBg && (!isOnEdge || registers.ppuMask.showBg8);

    unsigned patternPosition = 15 - (((renderingPositionX & 7) + 8 * !!(renderingPositionX * 7)) & 0xF);
    unsigned pixel = 0;
    unsigned attributes = 0;
    if(showBackground) {
        pixel = (bgShiftPattern >> (patternPosition * 2)) & 3;
        attributes = (bgShiftAttributes >> (patternPosition * 2)) & (pixel > 0 ? 3 : 0);
    } else if((registers.ppuAddr & 0x3F00) == 0x3F00 && !registers.ppuMask.showBgSp) {
        pixel = registers.ppuAddr;
    }

    pixel = palette[(attributes * 4 + pixel) & 0x1F] & (registers.ppuMask.greyscale ? 0x30 : 0x3F);
    framebuffer->setColor(renderingPositionX, scanline, pixel);
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

    cartridge->write(addr, value);
}
